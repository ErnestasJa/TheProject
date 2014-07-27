#include "Precomp.h"
#include "SGScenegraphLoader.h"
#include "SGScenegraph.h"
#include "SGEmptyObject.h"
#include "application/AppContext.h"
#include "physics/MotionState.h"
#include "json.h"

namespace sg
{

ScenegraphLoader::ScenegraphLoader()
{
    //ctor
}

ScenegraphLoader::~ScenegraphLoader()
{
    //dtor
}

glm::vec3 JsonReadVec3(const Json::Value & val);
glm::quat JsonReadQuat(const Json::Value & val);
void load_physics_for(AppContext * app_ctx, sg_object_ptr obj, const Json::Value & el);
bool ScenegraphLoader::LoadScene(AppContext * app_ctx, const std::string & filename,bool with_physics)
{
    std::string scene_path = filename.substr(0,filename.rfind("/")+1);
    std::cout << "scene_path =" << scene_path.c_str() << std::endl;

    Json::Value root;
    Json::Reader reader;

    uint32_t len;
    char * buf;
    len = helpers::read(filename, buf);

    if(len==0)
        return false;

    bool success = reader.parse(buf, buf+len, root, false);

    if(!success)
        return false;

    Json::Value scene = root["scene"];

    std::string collisionShapeType = "NULL";
    sg_object_ptr sgobj;

    for( uint32_t i = 0; i < scene.size(); i++)
    {
        Json::Value object = scene[i];
        isg_object * o = nullptr;

        if(object.isNull()) continue;

        std::string type = object["type"].asString();
        std::string name = object["name"].asString();
        std::cout <<"Got object [" << name << std::endl;

        if(type == "MESH")
        {
            std::string file = object["iqm_path"].asString();
            std::cout << "File = " << file << std::endl;

            sg::sg_mesh_object_ptr  obj = app_ctx->_scenegraph->load_mesh_object(scene_path+"/"+file,true);
            sgobj = obj;
            o = obj.get();

            app_ctx->_scenegraph->add_object(obj);
        }
        else if(type == "LAMP")
        {
            sg::sg_light_object_ptr  obj = app_ctx->_scenegraph->add_light_object();
            o = obj.get();
        }
        else if(type == "CAMERA")
        {
            sg::sg_camera_object_ptr  obj = sg_camera_object_ptr(new sg_camera_object(app_ctx,glm::vec3(0,0,0),glm::vec3(0,0,-1),glm::vec3(0,1,0)));
            o = obj.get();

            app_ctx->_scenegraph->add_object(obj);
        }
        else if(type == "EMPTY")
        {
            sg::sg_empty_object_ptr obj=share(new sg::sg_empty_object(app_ctx->_scenegraph));
            app_ctx->_scenegraph->add_trigger(obj,name);
            o = obj.get();
        }
        else
            continue;

        o->SetName(name);

        Json::Value val = object["position"];
        if(!val.isNull())
            o->set_position(JsonReadVec3(val));
        else
            throw "nope.avi";

        val = object["rotation"];
        if(!val.isNull())
            o->set_rotation(glm::quat(JsonReadVec3(val)));
        else
            throw "nope.avi";

        val = object["scale"];
        if(!val.isNull())
            o->set_scale(JsonReadVec3(val));
        else
            throw "nope.avi";

        if(with_physics && sgobj->get_type() == SGO_MESH)
            load_physics_for(app_ctx, sgobj, object);
    }

    return true;
}

glm::vec3 JsonReadVec3(const Json::Value & val)
{
    glm::vec3 vec;
    vec.x = val["x"].asFloat();
    vec.y = val["y"].asFloat();
    vec.z = val["z"].asFloat();
    return vec;
}

glm::quat JsonReadQuat(const Json::Value & val)
{
    glm::quat quat;
    quat.w = val["w"].asFloat();
    quat.x = val["x"].asFloat();
    quat.y = val["y"].asFloat();
    quat.z = val["z"].asFloat();
    return quat;
}

void load_physics_for(AppContext * app_ctx, sg_object_ptr obj, const Json::Value & el)
{
    if(obj)
    {
        Json::Value physics_data = el["physics_data"];
        if(physics_data.isNull()) return;

        PhysicsManager * pmgr = app_ctx->_physicsManager;

        std::string collisionShapeType = "NULL";
        std::string physicsType = "STATIC";
        btCollisionShape * collisionShape = nullptr;
        btVector3 localInertia;
        float radius=1.0f;
        float mass=1.0f;

        mass = physics_data["mass"].asFloat();
        radius = physics_data["radius"].asFloat();
        collisionShapeType = physics_data["collision_bounds_type"].asString();
        physicsType = physics_data["physics_type"].asString();

        if(collisionShapeType == "SPHERE")
            collisionShape = pmgr->createSphereShape(radius);
        else if(collisionShapeType == "BOX")
        {
            btVector3 extents(1.0f, 1.0f, 1.0f);
            AABB aabb = obj->GetAABB();
            collisionShape = pmgr->createBoxShape(PhysicsManager::glm_to_bt(aabb.get_extent()));
        }
        else if(collisionShapeType == "TRIANGLE_MESH")
        {
            if(obj->get_type()==SGO_MESH)
                collisionShape = pmgr->createTrimeshShape(static_cast<sg_mesh_object*>(obj.get())->get_mesh());
        }
        else if(collisionShapeType == "CONVEX_HULL")
        {
            throw "CONVEX_HULL shape type not supported";
        }
        else
            throw collisionShapeType;

        if(collisionShape)
        {
            btTransform transform;
            transform.setOrigin(PhysicsManager::glm_to_bt(obj->get_position()));
            transform.setRotation(PhysicsManager::quat_glm_to_bt(obj->get_rotation()));

            if(physicsType == "STATIC")
                mass = 0;
            else
                collisionShape->calculateLocalInertia(mass, localInertia);

            btRigidBody *rb = pmgr->createRigidBody(btRigidBody::btRigidBodyConstructionInfo(mass, new cmotion_state(transform, obj), collisionShape, localInertia));
            pmgr->addRigidBodyToWorld(rb);
        }

    }
}
}
