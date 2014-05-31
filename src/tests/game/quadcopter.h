#pragma once
#include "scenegraph/isg_object.h"
class btRigidBody;
class btVector3;

enum quad_model
{
    QUAD_MODEL_DEFAULT,
    QUAD_MODEL_FAST,
    QUAD_MODEL_UNSHIELDED,
    QUAD_MODEL_MICRO
};

namespace sg
{
class quadcopter : public isg_object
{
private:
    float m_mass,m_speed,m_height,m_power;
    btVector3 m_rotation,m_force,m_transform;

    btRigidBody* m_body;
public:
    quadcopter(scenegraph* scenegraph,quad_model model,btVector3 init_pos,float init_height,btVector3 init_rotation,bool start_on);
    ~quadcopter();

    void spawn(scenegraph* scenegraph,quad_model);

    void variate_height(float step);
    void variate_gyro(const btVector3 &gyro_step);

    float raycast_distance();

    void update(float delta);

    void set_position(const glm::vec3 &v);
};
}
