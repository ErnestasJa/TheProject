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

    void get_colliding_bodies(btDynamicsWorld *world);

    btRigidBody* create_box(MeshPtr mesh_obj, const btScalar mass, bool is_static, bool is_kinematic);
    btRigidBody* create_trimesh_body(MeshPtr mesh_obj, const btVector3 & scale);
    ///Conversion
    btTriangleMesh* convert_mesh_to_bullet_trimesh(MeshPtr pmesh, const btVector3 & scaling);

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
private:
    btVector3 m_gravity;
    btSoftBodyWorldInfo	m_softBodyWorldInfo;

    std::vector<btRigidBody *> m_rigidObjects;
    std::vector<btSoftBody *> m_softObjects;

    icollision_handler                           * m_handler;

    btDiscreteDynamicsWorld                    * m_world;
    btCollisionDispatcher                       * m_dispatcher;
    btBroadphaseInterface                       * m_broadPhase;
    btDefaultCollisionConfiguration   * m_collisionConfiguration;
    btConstraintSolver                          * m_solver;
    btGhostPairCallback                         * m_ghostPairCallback;
};

void myTickCallback(btDynamicsWorld *world, btScalar timeStep);

#endif // PHYSICSMANAGER_H
