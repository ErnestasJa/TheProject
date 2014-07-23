#include "Precomp.h"
#include "MotionState.h"
#include "CollisionHandler.h"
#include "PhysicsManager.h"
#include <BulletSoftBody/btSoftBodyRigidBodyCollisionConfiguration.h>
#include <BulletSoftBody/btSoftBodyHelpers.h>

#include "scenegraph/SGObjects.h"
#include "opengl/Mesh.h"
#include "opengl/BufferObject.h"

PhysicsManager::PhysicsManager(btVector3 Gravity):m_handler(0)
{
    m_collisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    m_broadPhase = new btDbvtBroadphase();
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

PhysicsManager::~PhysicsManager()
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

void PhysicsManager::clear()
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

void PhysicsManager::destroy_body(btRigidBody * body)
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

void PhysicsManager::update(float delta_time)
{
    m_world->stepSimulation(delta_time, 7);
}

void PhysicsManager::get_colliding_bodies(btDynamicsWorld *world)
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

btCollisionShape * PhysicsManager::createBoxShape(const btVector3 & extents)
{
    btCollisionShape * shape = new btBoxShape(extents * btVector3(0.5,0.5,0.5));
    return shape;
}

btCollisionShape * PhysicsManager::createSphereShape(btScalar radius)
{
    btCollisionShape * shape = new btSphereShape(radius);
    return shape;
}

btCollisionShape * PhysicsManager::createTrimeshShape(MeshPtr mesh, const btVector3 & scaling)
{
    btTriangleMesh* indexVertexArrays = ConvertMeshToBulletTrimesh(mesh, scaling);
    btBvhTriangleMeshShape * shape = new btBvhTriangleMeshShape(indexVertexArrays,true);
    return shape;
}

btRigidBody * PhysicsManager::createRigidBody(const btRigidBody::btRigidBodyConstructionInfo & info)
{
    return new btRigidBody(info);
}

void PhysicsManager::addRigidBodyToWorld(btRigidBody * rigidBody)
{
    m_world->addRigidBody(rigidBody);
}

void PhysicsManager::remove_and_delete_object(btCollisionObject * obj)
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

void PhysicsManager::remove_object_from_simulation(btCollisionObject * obj)
{
    this->m_world->removeCollisionObject(obj);
}

btTriangleMesh* PhysicsManager::ConvertMeshToBulletTrimesh(MeshPtr pmesh, const btVector3 & scaling)
{
    btVector3 vertices[3];
    btTriangleMesh * pTriMesh = nullptr;

    if(pmesh)
    {
        pTriMesh = new btTriangleMesh();
        buffer_object<glm::vec3> * bo = static_cast<buffer_object<glm::vec3> *>(pmesh->buffers[Mesh::POSITION]);
        index_buffer_object<uint32_t> * ibo = static_cast<index_buffer_object<uint32_t> *>(pmesh->buffers[Mesh::INDICES]);

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

std::vector<btRigidBody *> & PhysicsManager::getObjectList()
{
    return m_rigidObjects;
}

btSoftRigidDynamicsWorld* PhysicsManager::get_world()
{
    return (btSoftRigidDynamicsWorld*)m_world;
}

void myTickCallback(btDynamicsWorld *world, btScalar timeStep)
{
    PhysicsManager * physicsManager = static_cast<PhysicsManager*>(world->getWorldUserInfo());
    physicsManager->get_colliding_bodies(world);
}

///RayCast
btRigidBody * PhysicsManager::getRayCastResult(const btVector3 & start,const btVector3 & end)
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

btRigidBody * PhysicsManager::getRayCastResult(const btVector3 & start,const btVector3 & end, float & distance)
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

#include "CollisionFilterResultCallbackOnlyThis.h"
///Filtered rayCast
btRigidBody * PhysicsManager::getFilteredRayCastResult(const btVector3 & start,const btVector3 & end, short int filter)
{
    btRigidBody * body;
    CollisionFilterResultCallbackOnlyThis cb(start,end,filter);
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

btRigidBody * PhysicsManager::getFilteredRayCastResult(const btVector3 & start,const btVector3 & end, short int filter, float & distance)
{
    btRigidBody * body;
    CollisionFilterResultCallbackOnlyThis cb(start,end,filter);
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

void PhysicsManager::set_collision_handler(icollision_handler * handler)
{
    m_handler=handler;
}

void PhysicsManager::QuaternionToEulerXYZ(const btQuaternion &quat,btVector3 &euler)
{
    float w=quat.getW();
    float x=quat.getX();
    float y=quat.getY();
    float z=quat.getZ();
    double sqw = w*w;
    double sqx = x*x;
    double sqy = y*y;
    double sqz = z*z;
    euler.setZ((atan2(2.0 * (x*y + z*w),(sqx - sqy - sqz + sqw))));
    euler.setX((atan2(2.0 * (y*z + x*w),(-sqx - sqy + sqz + sqw))));
    euler.setY((asin(-2.0 * (x*z - y*w))));
}

void PhysicsManager::EulerXYZToQuaternion(btVector3 &euler, btQuaternion &quat)
{
    btMatrix3x3 mat;
    mat.setIdentity();
    mat.setEulerZYX(euler.getX(), euler.getY(), euler.getZ());
    mat.getRotation(quat);
}

const glm::vec3 PhysicsManager::bt_to_glm(const btVector3 & vec)
{
    return glm::vec3(vec.getX(),vec.getY(),vec.getZ());
}

const btVector3 PhysicsManager::glm_to_bt(const glm::vec3 & vec)
{
    return btVector3(vec.x,vec.y,vec.z);
}

const btVector3 PhysicsManager::glm_to_bt(const glm::vec4 & vec)
{
    return btVector3(vec.x,vec.y,vec.z);
}

const btQuaternion PhysicsManager::quat_glm_to_bt(const glm::quat & vec)
{
    return btQuaternion(vec.x,vec.y,vec.z,vec.w);
}
