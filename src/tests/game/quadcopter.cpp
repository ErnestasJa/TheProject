#include "precomp.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/sg_graphics_manager.h"
#include "tests/game/physics/physics.h"

#include "quadcopter.h"
using namespace sg;
quadcopter::quadcopter(scenegraph* scenegraph,quad_model model,btVector3 init_pos,float init_height,btVector3 init_rotation,bool start_on):isg_object(scenegraph)
{
    sg_graphics_manager_ptr gm=scenegraph->get_graphics_manager();

    switch(model)
    {
    case QUAD_MODEL_DEFAULT:
        m_mass=25;
        m_speed=10;
        m_height=init_height;
        m_power=1;
        break;
    case QUAD_MODEL_UNSHIELDED:
        m_mass=15;
        m_speed=15;
        m_height=init_height;
        m_power=1;
        break;
    case QUAD_MODEL_MICRO:
        m_mass=10;
        m_speed=12;
        m_height=init_height;
        m_power=1;
        break;
    default:
        break;
    }


}

quadcopter::~quadcopter()
{

}

void quadcopter::variate_height(float step)
{

}

void quadcopter::variate_gyro(const btVector3 & gyro_step)
{

}

float quadcopter::raycast_distance()
{
    return 0;
}

void quadcopter::update(float delta)
{

}

void quadcopter::set_position(const glm::vec3 &v)
{

}
