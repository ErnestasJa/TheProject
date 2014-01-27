#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <string>


class ICollisionHandler;

class CMotionState;
class btSoftBodyRigidBodyCollisionConfiguration;


class PhysicsManager
{
public:
    PhysicsManager(btVector3 Gravity);
    ~PhysicsManager();
    ///update stuff
    void update(float delta_time);
    void updateNodeTransformations();
    void updateRender(btRigidBody* body);

    ///rope rendering is mostly for debug purposes, would need to extend somehow softbody implementation to add custom materials.
    ///Also should use triangles to render them instead of lines.
    void renderRopes();

    void getCollidingBodies(btDynamicsWorld *world);

    ///Conversion
    btTriangleMesh* convert_mesh_to_bullet_trimesh(mesh_ptr pmesh, const btVector3 & scaling);
    static const glm::vec3 bt_to_glm(const btVector3 & vec)
    {
        return glm::vec3(vec.getX(),vec.getY(),vec.getZ());
    }
    static const btVector3 glm_to_bt(const glm::vec3 & vec)
    {
        return btVector3(vec.x,vec.y,vec.z);
    }

    static const btVector3 glm_to_bt(const glm::vec4 & vec)
    {
        return btVector3(vec.x,vec.y,vec.z);
    }

    static const btQuaternion quat_glm_to_bt(const glm::quat & vec)
    {
        return btQuaternion(vec.x,vec.y,vec.z,vec.w);
    }

    ///Physics body creation/destruction
    btRigidBody* createBox          (sg::sg_mesh_object_ptr mesh_obj, const btScalar mass, bool is_static=false, bool is_kinematic=false);
    btRigidBody* createSphere       (sg::sg_mesh_object_ptr mesh_obj, const btScalar mass, bool is_static=false, bool is_kinematic=false);
    btRigidBody* createTrimeshBody  (sg::sg_mesh_object_ptr mesh_obj, const btVector3 & scale);
    btSoftBody * createRope         (const btVector3 & start, const btVector3 & end, const btScalar nodes, int fixeds=1);
    void destroyBody(btRigidBody * body);

    ///removes body from simulation but does not delete it.
    void addObjectBackToSimulation(btCollisionObject * obj);
    ///removes body from simulation but does not delete it.
    void removeObjectFromSimulation(btCollisionObject * obj);
    ///removes body from simulation and deletes it.
    void removeAndDeleteObject(btCollisionObject * obj);

    ///Setters
    void setCollisionHandler(ICollisionHandler * handler)
    {
        m_handler=handler;
    }

    ///Geters
    btSoftRigidDynamicsWorld * getWorld();
    std::vector<btRigidBody *> & getObjectList();

    ///RayCast
    btRigidBody * getRayCastResult(const btVector3 & start,const btVector3 & end);
    btRigidBody * getRayCastResult(const btVector3 & start,const btVector3 & end, float & distance);
    btRigidBody * getFilteredRayCastResult(const btVector3 & start,const btVector3 & end, short int filter);
    btRigidBody * getFilteredRayCastResult(const btVector3 & start,const btVector3 & end, short int filter, float & distance);

private:
    btVector3 m_gravity;
    btSoftBodyWorldInfo	m_softBodyWorldInfo;
    std::vector<btRigidBody *> m_rigidObjects;
    std::vector<btSoftBody *> m_softObjects;

    ICollisionHandler                           * m_handler;

    btSoftRigidDynamicsWorld                    * m_world;
    btCollisionDispatcher                       * m_dispatcher;
    btBroadphaseInterface                       * m_broadPhase;
    btSoftBodyRigidBodyCollisionConfiguration   * m_collisionConfiguration;
    btConstraintSolver                          * m_solver;
    btGhostPairCallback                         * m_ghostPairCallback;
};

void myTickCallback(btDynamicsWorld *world, btScalar timeStep);

///Ray test callback that should get only body that mathces the filter.
///Not much tested but I think it works.
struct	CFRC_OnlyThis : public btCollisionWorld::RayResultCallback
{
    CFRC_OnlyThis(const btVector3&	rayFromWorld,const btVector3&	rayToWorld, short int filter)
        :m_rayFromWorld(rayFromWorld),
         m_rayToWorld(rayToWorld),
         m_filter(filter)
    {
    }



    btVector3	m_rayFromWorld;//used to calculate hitPointWorld from hitFraction
    btVector3	m_rayToWorld;

    btVector3	m_hitNormalWorld;
    btVector3	m_hitPointWorld;

    short int   m_filter;

    virtual	btScalar	addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
    {
        //caller already does the filter on the m_closestHitFraction
        btAssert(rayResult.m_hitFraction <= m_closestHitFraction);

        /*if(!m_collisionObject)
        {

            m_collisionObject = rayResult.m_collisionObject;
            if((rayResult.m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup&m_filter)==m_filter)
                m_closestHitFraction = 0;
            else
                m_closestHitFraction = rayResult.m_hitFraction;

        }
        else*/ if((rayResult.m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup&m_filter)==m_filter)
        {
            m_closestHitFraction = rayResult.m_hitFraction;
            m_collisionObject = rayResult.m_collisionObject;
        }
        /*else if(!((m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup&m_filter)==m_filter))
        {
            m_closestHitFraction = rayResult.m_hitFraction;
            m_collisionObject = rayResult.m_collisionObject;
        }*/


        if (normalInWorldSpace)
        {
            m_hitNormalWorld = rayResult.m_hitNormalLocal;
        }
        else
        {
            ///need to transform normal into worldspace
            m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
        }
        m_hitPointWorld.setInterpolate3(m_rayFromWorld,m_rayToWorld,rayResult.m_hitFraction);

        return m_closestHitFraction;
    }
};

#endif // PHYSICSMANAGER_H
