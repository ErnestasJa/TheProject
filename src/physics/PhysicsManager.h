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


class PhysicsManager
{
    ///Also should use triangles to Render them instead of lines.
private:
    btVector3 m_gravity;
    btSoftBodyWorldInfo	m_softBodyWorldInfo;
    std::vector<btRigidBody *> m_rigidObjects;
    std::vector<btSoftBody *> m_softObjects;

    icollision_handler                          * m_handler;
    btSoftRigidDynamicsWorld                    * m_world;
    btCollisionDispatcher                       * m_dispatcher;
    btBroadphaseInterface                       * m_broadPhase;
    btSoftBodyRigidBodyCollisionConfiguration   * m_collisionConfiguration;
    btConstraintSolver                          * m_solver;
    btGhostPairCallback                         * m_ghostPairCallback;

public:
    PhysicsManager(btVector3 Gravity);
    ~PhysicsManager();

    btTriangleMesh* ConvertMeshToBulletTrimesh(MeshPtr pmesh, const btVector3 & scaling);

    btCollisionShape * createBoxShape(const btVector3 & extents);
    btCollisionShape * createSphereShape(btScalar radius);
    btCollisionShape * createTrimeshShape(MeshPtr mesh, const btVector3 & scaling = btVector3(1.0f,1.0f,1.0f));
    btRigidBody      * createRigidBody(const btRigidBody::btRigidBodyConstructionInfo & info);
    void               addRigidBodyToWorld(btRigidBody * rigidBody);

    void add_object_back_to_simulation(btCollisionObject * obj);
    btRigidBody * getRayCastResult(const btVector3 & start,const btVector3 & end);
    btRigidBody * getRayCastResult(const btVector3 & start,const btVector3 & end, float & distance);
    btRigidBody * getFilteredRayCastResult(const btVector3 & start,const btVector3 & end, short int filter);
    btRigidBody * getFilteredRayCastResult(const btVector3 & start,const btVector3 & end, short int filter, float & distance);
    void get_colliding_bodies(btDynamicsWorld *world);

    void clear();
    void update(float delta_time);
    void update_node_transformations();
    void update_render(btRigidBody* body);

    void destroy_body(btRigidBody * body);
    void remove_object_from_simulation(btCollisionObject * obj);
    void remove_and_delete_object(btCollisionObject * obj);

    btSoftRigidDynamicsWorld * get_world();
    std::vector<btRigidBody *> & getObjectList();
    void set_collision_handler(icollision_handler * handler);
public:

    static void QuaternionToEulerXYZ(const btQuaternion &quat,btVector3 &euler);
    static void EulerXYZToQuaternion(btVector3 &euler, btQuaternion &quat);
    static const glm::vec3 bt_to_glm(const btVector3 & vec);
    static const btVector3 glm_to_bt(const glm::vec3 & vec);
    static const btVector3 glm_to_bt(const glm::vec4 & vec);
    static const btQuaternion quat_glm_to_bt(const glm::quat & vec);

};

void myTickCallback(btDynamicsWorld *world, btScalar timeStep);

#endif // PHYSICSMANAGER_H
