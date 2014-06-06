#include "precomp.h"
#include "cmotion_state.h"
#include "icollision_handler.h"
#include "physics_manager.h"
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>

#include "scenegraph/sg_objects.h"
#include "opengl/mesh.h"
#include "opengl/buffer_object.h"

physics_manager::physics_manager(btVector3 Gravity):m_handler(0)
{
    m_collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    m_broadPhase = new btDbvtBroadphase();
    //m_ghostPairCallback = new btGhostPairCallback();
    //m_broadPhase->getOverlappingPairCache()->setInternalGhostPairCallback(m_ghostPairCallback);
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver();
    m_world = new btSoftRigidDynamicsWorld(m_dispatcher, m_broadPhase, m_solver, m_collisionConfiguration);
    m_world->setGravity(Gravity);

    m_softBodyWorldInfo.m_broadphase=m_broadPhase;
    m_softBodyWorldInfo.m_dispatcher=m_dispatcher;
    m_softBodyWorldInfo.m_sparsesdf.Initialize();
    m_softBodyWorldInfo.m_gravity=Gravity;
    m_softBodyWorldInfo.air_density = 1.0f;
    m_softBodyWorldInfo.water_density = 0;
    m_softBodyWorldInfo.water_offset = 0;
    m_softBodyWorldInfo.water_normal = btVector3(0,0,0);

    m_world->setInternalTickCallback(myTickCallback,(void*)this,true);
}

physics_manager::~physics_manager()
{
    m_softBodyWorldInfo.m_sparsesdf.GarbageCollect();
    m_softBodyWorldInfo.m_sparsesdf.Reset();

    std::vector<btRigidBody*>::iterator it;
    for (it=m_rigidObjects.begin(); it!=m_rigidObjects.end();)
    {
        btRigidBody * body =(*it);
        m_world->removeRigidBody((btRigidBody*)body);
        destroy_body(body);
        it=m_rigidObjects.erase(it);
    }
    std::vector<btSoftBody*>::iterator it2;
    for (it2=m_softObjects.begin(); it2!=m_softObjects.end();)
    {
        btSoftBody * body =(*it2);
        get_world()->removeSoftBody((btSoftBody*)body);
        delete body;
        it2=m_softObjects.erase(it2);
    }

    if(m_world)
        delete m_world;

    if(m_solver)
        delete m_solver;

    if(m_broadPhase)
        delete m_broadPhase;

    if(m_dispatcher)
        delete m_dispatcher;

    if(m_collisionConfiguration)
        delete m_collisionConfiguration;

    printf("~physics_manager\n");
}

void physics_manager::clear()
{
    m_softBodyWorldInfo.m_sparsesdf.GarbageCollect();
    m_softBodyWorldInfo.m_sparsesdf.Reset();

    std::vector<btRigidBody*>::iterator it;
    for (it=m_rigidObjects.begin(); it!=m_rigidObjects.end();)
    {
        btRigidBody * body =(*it);
        m_world->removeRigidBody((btRigidBody*)body);
        destroy_body(body);
        it=m_rigidObjects.erase(it);
    }
    m_rigidObjects.clear();
    std::vector<btSoftBody*>::iterator it2;
    for (it2=m_softObjects.begin(); it2!=m_softObjects.end();)
    {
        btSoftBody * body =(*it2);
        get_world()->removeSoftBody((btSoftBody*)body);
        delete body;
        it2=m_softObjects.erase(it2);
    }
    m_softObjects.clear();
}

void physics_manager::destroy_body(btRigidBody * body)
{
    if(body->getCollisionShape())
    {
        if(body->getCollisionShape()->getShapeType() == TRIANGLE_MESH_SHAPE_PROXYTYPE)
        {
            btTriangleMeshShape * shape = (btBvhTriangleMeshShape*)body->getCollisionShape();
            delete shape->getMeshInterface();
        }
        delete body->getCollisionShape();
    }

    if(body->getMotionState())
    {
        delete body->getMotionState();
    }
    delete body;
}

void physics_manager::update(float delta_time)
{
    m_world->stepSimulation(delta_time, 7);
}

void physics_manager::render_ropes()
{
    /*
    irr::video::SMaterial mat;
    mat.Lighting=false;
    driver->setMaterial(mat);
    driver->setTransform(irr::video::ETS_WORLD, irr::core::IdentityMatrix);
    btSoftBodyArray&         softbodies(this->get_world()->getSoftBodyArray());
    //For each soft bodies
    for(int i=0; i<softbodies.size(); ++i)
    {

        btSoftBody*            softbody(softbodies[i]);
        //Each soft body contain an array of vertices (nodes/particles_mass)

        btSoftBody::tLinkArray&   links(softbody->m_links);


        for(int j=0; j<links.size(); ++j)
        {
            btSoftBody::Node*   node_0=links[j].m_n[0];
            btSoftBody::Node*   node_1=links[j].m_n[1];

            driver->draw3DLine(utils::BtVecToIrrVec(node_1->m_x), utils::BtVecToIrrVec(node_0->m_x),irr::video::SColor(255,255,255,255));
        }
    }
    */
}

void physics_manager::get_colliding_bodies(btDynamicsWorld *world)
{
    int numManifolds = world->getDispatcher()->getNumManifolds();
    for (int i=0; i<numManifolds; i++)
    {
        btPersistentManifold* contactManifold =  world->getDispatcher()->getManifoldByIndexInternal(i);
        const btRigidBody * body1=static_cast<const btRigidBody*>(contactManifold->getBody0());
        const btRigidBody * body2=static_cast<const btRigidBody*>(contactManifold->getBody1());

        if(m_handler)
            m_handler->on_body_collide(body1,body2);
    }
}

btRigidBody* physics_manager::create_box(sg::sg_mesh_object_ptr mesh_obj, const btScalar mass, bool is_static, bool is_kinematic)
{
    if(!mesh_obj)
        return NULL;

    btVector3 HalfExtents
    (
        mesh_obj->get_aabb().get_extent().x * 0.5f,
        mesh_obj->get_aabb().get_extent().y * 0.5f,
        mesh_obj->get_aabb().get_extent().z * 0.5f
    );

    std::cout << "Half extents: x = " << HalfExtents.getX() << " y = " << HalfExtents.getY() << " z = " << HalfExtents.getZ() << std::endl;

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(glm_to_bt(mesh_obj->get_position()));
    Transform.setRotation(quat_glm_to_bt(glm::quat_cast(mesh_obj->get_absolute_transform())));

    cmotion_state *MotionState = new cmotion_state(Transform, mesh_obj);
    btCollisionShape *Shape = new btBoxShape(HalfExtents);

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(mass, LocalInertia);

    btRigidBody *RigidBody = new btRigidBody( mass, MotionState, Shape, LocalInertia);

    RigidBody->setActivationState(DISABLE_DEACTIVATION);

    if(is_static)
        RigidBody->setCollisionFlags(RigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
    if(is_kinematic)
        RigidBody->setCollisionFlags(RigidBody->getCollisionFlags() | btRigidBody::CF_KINEMATIC_OBJECT);

    RigidBody->setCcdMotionThreshold(1);
    RigidBody->setCcdSweptSphereRadius(mesh_obj->get_aabb().get_extent().x*0.2f);

    m_world->addRigidBody(RigidBody);
    m_rigidObjects.push_back(RigidBody);
    return RigidBody;
}

btRigidBody* physics_manager::create_sphere(sg::sg_mesh_object_ptr mesh_obj, const btScalar mass, bool is_static, bool is_kinematic)
{
    if(!mesh_obj)
        return NULL;

    btVector3 HalfExtents
    (
        mesh_obj->get_aabb().get_extent().x * 0.5f,
        mesh_obj->get_aabb().get_extent().y * 0.5f,
        mesh_obj->get_aabb().get_extent().z * 0.5f
    );

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(glm_to_bt(mesh_obj->get_position()));
    Transform.setRotation(quat_glm_to_bt(glm::quat_cast(mesh_obj->get_absolute_transform())));

    cmotion_state *MotionState = new cmotion_state(Transform, mesh_obj);
    btCollisionShape *Shape = new btSphereShape(mesh_obj->get_aabb().get_extent().x*0.5f);

    btVector3 LocalInertia;
    Shape->calculateLocalInertia(mass, LocalInertia);

    btRigidBody *RigidBody = new btRigidBody(mass, MotionState, Shape, LocalInertia);

    //RigidBody->setActivationState(DISABLE_DEACTIVATION);
    if(is_static)
        RigidBody->setCollisionFlags(RigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
    if(is_kinematic)
        RigidBody->setCollisionFlags(RigidBody->getCollisionFlags() | btRigidBody::CF_KINEMATIC_OBJECT);

    m_world->addRigidBody((btRigidBody*)RigidBody);
    m_rigidObjects.push_back(RigidBody);
    return RigidBody;
}

btRigidBody* physics_manager::create_trimesh_body(sg::sg_mesh_object_ptr mesh_obj, const btVector3 & scale)
{
    if(!mesh_obj)
        return NULL;

    btTriangleMesh* indexVertexArrays = convert_mesh_to_bullet_trimesh(mesh_obj->get_mesh(), scale);
    btBvhTriangleMeshShape * trimeshShape = new btBvhTriangleMeshShape(indexVertexArrays,true);

    btTransform Transform;
    Transform.setIdentity();
    Transform.setOrigin(glm_to_bt(mesh_obj->get_position()));
    glm::quat sq=mesh_obj->get_rotation();
    btQuaternion quatty=btQuaternion(sq.x,sq.y,sq.z,sq.w);
    Transform.setRotation(quatty);

    cmotion_state *MotionState = new cmotion_state(Transform, mesh_obj);

    btRigidBody *RigidBody=NULL;

    RigidBody = new btRigidBody(0, MotionState, trimeshShape);
    RigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);


    RigidBody->activate();
    m_world->addRigidBody((btRigidBody*)RigidBody);
    m_rigidObjects.push_back(RigidBody);

    return RigidBody;
}

btSoftBody* physics_manager::create_rope(const btVector3 & start, const btVector3 & end, const btScalar nodes, int fixeds)
{
    btSoftBody*		psb=btSoftBodyHelpers::CreateRope(m_softBodyWorldInfo,start,end,nodes,fixeds);
    m_softObjects.push_back(psb);
    get_world()->addSoftBody(psb);

    return psb;
}

void physics_manager::remove_and_delete_object(btCollisionObject * obj)
{
    this->m_world->removeCollisionObject(obj);
    if(obj->getInternalType()== btCollisionObject::CO_RIGID_BODY)
    {
        std::vector<btRigidBody*>::iterator it;
        for (it=m_rigidObjects.begin(); it!=m_rigidObjects.end(); it++)
        {
            btRigidBody * body =(*it);
            if(body==(btRigidBody*)obj)
            {
                it=m_rigidObjects.erase(it);
                destroy_body(body);
                return;
            }
        }
    }
    if(obj->getInternalType()== btCollisionObject::CO_SOFT_BODY)
    {
        std::vector<btSoftBody*>::iterator it2;
        for (it2=m_softObjects.begin(); it2!=m_softObjects.end(); it2++)
        {
            btSoftBody * body =(*it2);
            if(body==(btSoftBody*)obj)
            {
                it2=m_softObjects.erase(it2);
                delete body;
                return;
            }
        }

    }
}

void physics_manager::remove_object_from_simulation(btCollisionObject * obj)
{
    this->m_world->removeCollisionObject(obj);
}

btTriangleMesh* physics_manager::convert_mesh_to_bullet_trimesh(mesh_ptr pmesh, const btVector3 & scaling)
{
    btVector3 vertices[3];
    btTriangleMesh * pTriMesh = nullptr;

    if(pmesh)
    {
        pTriMesh = new btTriangleMesh();
        buffer_object<glm::vec3> * bo = static_cast<buffer_object<glm::vec3> *>(pmesh->buffers[mesh::POSITION]);
        index_buffer_object<uint32_t> * ibo = static_cast<index_buffer_object<uint32_t> *>(pmesh->buffers[mesh::INDICES]);

        if(bo->data.size()>0&&ibo->data.size()>0)

        for(uint32_t i = 0; i < ibo->data.size(); i+=3)
        {
            vertices[0] = glm_to_bt(bo->data[ibo->data[i]]) * scaling;
            vertices[1] = glm_to_bt(bo->data[ibo->data[i+1]]) * scaling;
            vertices[2] = glm_to_bt(bo->data[ibo->data[i+2]]) * scaling;

            pTriMesh->addTriangle(vertices[0], vertices[1], vertices[2]);
        }
    }

    return pTriMesh;
};

std::vector<btRigidBody *> & physics_manager::getObjectList()
{
    return m_rigidObjects;
}

btSoftRigidDynamicsWorld* physics_manager::get_world()
{
    return (btSoftRigidDynamicsWorld*)m_world;
}

void myTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    physics_manager * physicsManager = static_cast<physics_manager*>(world->getWorldUserInfo());
    physicsManager->get_colliding_bodies(world);
}

///RayCast
btRigidBody * physics_manager::getRayCastResult(const btVector3 & start,const btVector3 & end)
{
    btRigidBody * body;
    btCollisionWorld::ClosestRayResultCallback cb(start,end);
    this->get_world()->rayTest(start,end,cb);
    if(cb.hasHit())
    {
        if(cb.m_collisionObject->getInternalType()==btCollisionObject::CO_RIGID_BODY)
        {
            body=(btRigidBody*)cb.m_collisionObject;
            return body;
        }
    }
    return 0;
}

btRigidBody * physics_manager::getRayCastResult(const btVector3 & start,const btVector3 & end, float & distance)
{
    btRigidBody * body;
    btCollisionWorld::ClosestRayResultCallback cb(start,end);
    this->get_world()->rayTest(start,end,cb);
    if(cb.hasHit())
    {
        if(cb.m_collisionObject->getInternalType()==btCollisionObject::CO_RIGID_BODY)
        {
            body=(btRigidBody*)cb.m_collisionObject;
            distance=(cb.m_hitPointWorld-start).length();
            return body;
        }
    }
    return 0;
}

///Filtered rayCast
btRigidBody * physics_manager::getFilteredRayCastResult(const btVector3 & start,const btVector3 & end, short int filter)
{
    btRigidBody * body;
    CFRC_OnlyThis cb(start,end,filter);
    this->get_world()->rayTest(start,end,cb);
    if(cb.hasHit())
    {
        if(cb.m_collisionObject->getInternalType()==btCollisionObject::CO_RIGID_BODY)
        {
            body=(btRigidBody*)cb.m_collisionObject;
            return body;
        }
    }
    return 0;
}

btRigidBody * physics_manager::getFilteredRayCastResult(const btVector3 & start,const btVector3 & end, short int filter, float & distance)
{
    btRigidBody * body;
    CFRC_OnlyThis cb(start,end,filter);
    this->get_world()->rayTest(start,end,cb);
    if(cb.hasHit())
    {
        if(cb.m_collisionObject->getInternalType()==btCollisionObject::CO_RIGID_BODY)
        {
            body=(btRigidBody*)cb.m_collisionObject;
            distance=(cb.m_hitPointWorld-start).length();
            return body;
        }
    }
    return 0;
}

