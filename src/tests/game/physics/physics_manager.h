#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>
#include <BulletSoftBody/btSoftRigidDynamicsWorld.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <string>


class icollision_handler;

class cmotion_state;
class btSoftBodyRigidBodyCollisionConfiguration;


class physics_manager
{
public:
    physics_manager(btVector3 Gravity);
    ~physics_manager();
    void clear();
    ///update stuff
    void update(float delta_time);
    void update_node_transformations();
    void update_render(btRigidBody* body);

    ///rope rendering is mostly for debug purposes, would need to extend somehow softbody implementation to add custom materials.
    ///Also should use triangles to render them instead of lines.
    void render_ropes();

    void get_colliding_bodies(btDynamicsWorld *world);

    ///Conversion
    btTriangleMesh* convert_mesh_to_bullet_trimesh(mesh_ptr pmesh, const btVector3 & scaling);

    static void QuaternionToEulerXYZ(const btQuaternion &quat,btVector3 &euler)
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

    static void EulerXYZToQuaternion(btVector3 &euler, btQuaternion &quat)
    {
        btMatrix3x3 mat;
        mat.setIdentity();
        mat.setEulerZYX(euler.getX(), euler.getY(), euler.getZ());
        mat.getRotation(quat);
    }

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
    btRigidBody* create_box          (sg::sg_mesh_object_ptr mesh_obj, const btScalar mass, bool is_static=false, bool is_kinematic=false);
    btRigidBody* create_sphere       (sg::sg_mesh_object_ptr mesh_obj, const btScalar mass, bool is_static=false, bool is_kinematic=false);
    btRigidBody* create_trimesh_body  (sg::sg_mesh_object_ptr mesh_obj, const btVector3 & scale);
    btSoftBody * create_rope         (const btVector3 & start, const btVector3 & end, const btScalar nodes, int fixeds=1);
    void destroy_body(btRigidBody * body);


    void add_object_back_to_simulation(btCollisionObject * obj);

    ///removes body from simulation but does not delete it.
    void remove_object_from_simulation(btCollisionObject * obj);
    ///removes body from simulation and deletes it.
    void remove_and_delete_object(btCollisionObject * obj);

    ///Setters
    void set_collision_handler(icollision_handler * handler)
    {
        m_handler=handler;
    }

    ///Geters
    btSoftRigidDynamicsWorld * get_world();
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

    icollision_handler                           * m_handler;

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
