#include "Precomp.h"
#include "cmotion_state.h"
#include "icollision_handler.h"
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>

#include "opengl/Mesh.h"
#include "opengl/BufferObject.h"

#include "physics_manager.h"

physics_manager::physics_manager(btVector3 Gravity):m_handler(0)
{
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_broadPhase = new btDbvtBroadphase();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_solver = new btSequentialImpulseConstraintSolver();
    m_world = new btDiscreteDynamicsWorld(m_dispatcher, m_broadPhase, m_solver, m_collisionConfiguration);
    m_world->setGravity(Gravity);

    m_world->setInternalTickCallback(myTickCallback,(void*)this,true);
}

physics_manager::~physics_manager()
{
    std::vector<btRigidBody*>::iterator it;
    for (it=m_rigidObjects.begin(); it!=m_rigidObjects.end();)
    {
        btRigidBody * body =(*it);
        m_world->removeRigidBody((btRigidBody*)body);
        destroy_body(body);
        it=m_rigidObjects.erase(it);
    }

    if(m_world)
        delete m_world;

    if(m_solver)
        delete m_solver;

    if(m_broadPhase)
        delete m_broadPhase;

    if(m_dispatcher)
        delete m_dispatcher;

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

btRigidBody* physics_manager::create_box(MeshPtr mesh_obj, const btScalar mass, bool is_static, bool is_kinematic)
{
    if(!mesh_obj)
        return NULL;

    btVector3 HalfExtents
    (
        mesh_obj->aabb.GetHalfSize().x,
        mesh_obj->aabb.GetHalfSize().y,
        mesh_obj->aabb.GetHalfSize().z
    );

    std::cout << "Half extents: x = " << HalfExtents.getX() << " y = " << HalfExtents.getY() << " z = " << HalfExtents.getZ() << std::endl;

    btTransform Transform;
    Transform.setIdentity();
    //Transform.setOrigin(glm_to_bt(mesh_obj->get_position()));
    //Transform.setRotation(quat_glm_to_bt(glm::quat_cast(mesh_obj->get_absolute_transform())));

    cmotion_state *MotionState = new cmotion_state(Transform);//, mesh_obj);
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
    RigidBody->setCcdSweptSphereRadius(mesh_obj->aabb.GetHalfSize().x*2.f*0.2f);

    m_world->addRigidBody(RigidBody);
    m_rigidObjects.push_back(RigidBody);
    return RigidBody;
}

btRigidBody* physics_manager::create_trimesh_body(MeshPtr mesh_obj, const btVector3 & scale)
{
    if(!mesh_obj)
        return NULL;

    btTriangleMesh* indexVertexArrays = convert_mesh_to_bullet_trimesh(mesh_obj, scale);
    btBvhTriangleMeshShape * trimeshShape = new btBvhTriangleMeshShape(indexVertexArrays,true);

    btTransform Transform;
    Transform.setIdentity();
    //Transform.setOrigin(glm_to_bt(mesh_obj->get_position()));
    //glm::quat sq=mesh_obj->get_rotation();
    //btQuaternion quatty=btQuaternion(sq.x,sq.y,sq.z,sq.w);
    //Transform.setRotation(quatty);

    cmotion_state *MotionState = new cmotion_state(Transform);//, mesh_obj);

    btRigidBody *RigidBody=NULL;

    RigidBody = new btRigidBody(0, MotionState, trimeshShape);
    RigidBody->setCollisionFlags(btCollisionObject::CF_STATIC_OBJECT);


    RigidBody->activate();
    m_world->addRigidBody((btRigidBody*)RigidBody);
    m_rigidObjects.push_back(RigidBody);

    return RigidBody;
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
}

void physics_manager::remove_object_from_simulation(btCollisionObject * obj)
{
    this->m_world->removeCollisionObject(obj);
}

btTriangleMesh* physics_manager::convert_mesh_to_bullet_trimesh(MeshPtr pmesh, const btVector3 & scaling)
{
    btVector3 vertices[3];
    btTriangleMesh * pTriMesh = nullptr;

    if(pmesh)
    {
        pTriMesh = new btTriangleMesh();
        BufferObject<glm::vec3> * bo = static_cast<BufferObject<glm::vec3> *>(pmesh->buffers[Mesh::POSITION]);
        IndexBufferObject<uint32_t> * ibo = static_cast<IndexBufferObject<uint32_t> *>(pmesh->buffers[Mesh::INDICES]);

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
