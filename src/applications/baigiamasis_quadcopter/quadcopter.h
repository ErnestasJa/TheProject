#pragma once
#include "scenegraph/isg_object.h"
class btRigidBody;
class btVector3;
class app_context;

enum quad_model
{
    QUAD_MODEL_DEFAULT,
    QUAD_MODEL_FAST,
    QUAD_MODEL_UNSHIELDED,
    QUAD_MODEL_MICRO
};

namespace sg
{
class quadcopter
{
private:
    app_context* m_app_ctx;
    float m_mass,m_speed,m_height,m_power,m_delta;


    btRigidBody* m_body;

    bool m_running;
    int setup[3];
    bool make_a_quadcopter(std::string modelpath,std::string texpath,std::string soundpath);
public:
    btVector3 m_rotation,m_force,m_transform,m_old_transform;

    quadcopter(app_context* app_ctx,int model,btVector3 init_pos,float init_height,btVector3 init_rotation,bool start_on);
    ~quadcopter();

    void spawn(scenegraph* scenegraph,quad_model);

    void variate_height(float step);
    void variate_gyro(const glm::vec3 &gyro_step);
    void variate_axis(float rot);

    float raycast_distance();

    void update(float delta);

    void set_position(const glm::vec3 &v);
    glm::vec3 get_position();

    void set_rotation(glm::vec3 rot);
};
}
