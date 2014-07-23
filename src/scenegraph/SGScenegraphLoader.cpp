#include "Precomp.h"
#include "SGScenegraphLoader.h"
#include "SGScenegraph.h"
#include "SGEmptyObject.h"
#include "application/AppContext.h"
#include "physics/MotionState.h"

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

void load_physics_for(AppContext * app_ctx, sg_object_ptr obj, tinyxml2::XMLElement * el);
bool ScenegraphLoader::LoadScene(AppContext * app_ctx, const std::string & filename,bool with_physics)
{
    std::string scene_path = filename.substr(0,filename.rfind("/")+1);
    std::cout << "scene_path =" << scene_path.c_str() << std::endl;

    tinyxml2::XMLDocument doc;
    uint32_t len;
    char * buf;
    len = helpers::read(filename, buf);

    if(len==0)
        return false;

    doc.Parse(buf);

    tinyxml2::XMLElement * scene = doc.FirstChildElement("scene");
    tinyxml2::XMLElement * object = scene->FirstChildElement("object");

    std::string collisionShapeType = "NULL";
    sg_object_ptr sgobj;

    while(object)
    {
        isg_object * o = nullptr;

        if(object->Attribute("Type","MESH"))
        {
            if(tinyxml2::XMLElement * file = object->FirstChildElement("iqm_file"))
            {
                std::cout <<"Got object [" << object->Attribute("name") << "], file = " << file->Attribute("name") << std::endl;

                sg::sg_mesh_object_ptr  obj = app_ctx->_scenegraph->load_mesh_object(scene_path+"/"+file->Attribute("name"),true);
                sgobj = obj;
                o = obj.get();

                app_ctx->_scenegraph->add_object(obj);
            }
        }
        else if(object->Attribute("Type","LAMP"))
        {

            sg::sg_light_object_ptr  obj = app_ctx->_scenegraph->add_light_object();
            o = obj.get();
        }
        else if(object->Attribute("Type","CAMERA"))
        {
            sg::sg_camera_object_ptr  obj = sg_camera_object_ptr(new sg_camera_object(app_ctx,glm::vec3(0,0,0),glm::vec3(0,0,-1),glm::vec3(0,1,0)));
            o = obj.get();

            app_ctx->_scenegraph->add_object(obj);
        }
        else if(object->Attribute("Type","EMPTY"))
        {
            printf("ADDED EMPTY OBJ: %s\n",object->Attribute("name"));
            sg::sg_empty_object_ptr obj=share(new sg::sg_empty_object(app_ctx->_scenegraph));
            app_ctx->_scenegraph->add_trigger(obj,object->Attribute("name"));
            o = obj.get();
        }
        else
        {
            object = object->NextSiblingElement();
            continue;
        }

        o->SetName(object->Attribute("name"));

        if(tinyxml2::XMLElement * pos = object->FirstChildElement("position"))
        {
            o->set_position(glm::vec3(pos->FloatAttribute("x"),pos->FloatAttribute("y"),pos->FloatAttribute("z")));
        }
        else
            throw "nope.avi";

        if(tinyxml2::XMLElement * rot = object->FirstChildElement("rotation"))
        {
            o->set_rotation(glm::quat(glm::vec3(rot->FloatAttribute("x"),rot->FloatAttribute("y"),rot->FloatAttribute("z"))));
        }
        else if(tinyxml2::XMLElement * rot = object->FirstChildElement("quat_rotation"))
        {
            o->set_rotation(glm::quat(rot->FloatAttribute("w"),rot->FloatAttribute("x"),rot->FloatAttribute("y"),rot->FloatAttribute("z")));
        }
        else
            throw "nope.avi";

        if(tinyxml2::XMLElement * scale = object->FirstChildElement("scale"))
        {
            o->set_scale(glm::vec3(scale->FloatAttribute("x"),scale->FloatAttribute("y"),scale->FloatAttribute("z")));
        }
        else
            throw "nope.avi";

        if(with_physics && sgobj->get_type() == SGO_MESH)
            load_physics_for(app_ctx, sgobj,object);
        object = object->NextSiblingElement();
    }

    return true;
}

void load_physics_for(AppContext * app_ctx, sg_object_ptr obj, tinyxml2::XMLElement * el)
{
    if(obj)
    {
        tinyxml2::XMLElement * e = el->FirstChildElement("game");
        if(!e) return;
        e = e->FirstChildElement();
        if(!e) return;

        PhysicsManager * pmgr = app_ctx->_physicsManager;

        std::string collisionShapeType = "NULL";
        std::string physicsType = "STATIC";
        btCollisionShape * collisionShape = nullptr;
        btVector3 localInertia;
        float radius=1.0f;
        float mass=1.0f;

        while(e)
        {
            if(strcmp(e->Name(), "mass")==0)
                mass = e->FloatAttribute("value");
            else if(strcmp(e->Name(), "collision_bounds_type")==0)
                collisionShapeType = e->Attribute("value");
            else if(strcmp(e->Name(), "physics_type")==0)
                physicsType = e->Attribute("value");
            else if(strcmp(e->Name(), "radius")==0)
                radius = e->FloatAttribute("value");

            e = e->NextSiblingElement();
        }

        if(collisionShapeType == "SPHERE")
            collisionShape = pmgr->createSphereShape(radius);
        else if(collisionShapeType == "BOX")
        {
            btVector3 extents(1.0f, 1.0f, 1.0f);
            AABB aabb = obj->get_aabb();
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
