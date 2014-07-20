#include "Precomp.h"
#include "SGLightObject.h"

namespace sg
{

sg_light_object::sg_light_object(SGScenegraph * sg): isg_object(sg)
{
    m_look = glm::vec3(0,0,-1);
    m_up = glm::vec3(0,1,0);
}

sg_light_object::~sg_light_object()
{
    //dtor
}

void sg_light_object::Render(SGScenegraph * sg)
{

}

sg_aabb sg_light_object::get_aabb()
{
    return m_aabb;
}

uint32_t sg_light_object::get_type()
{
    return SGO_LIGHT;
}

sg_material_ptr sg_light_object::get_material(uint32_t index)
{
    return sg_material_ptr();
}

bool sg_light_object::set_material(uint32_t index, sg_material_ptr mat)
{
    return false;
}

uint32_t sg_light_object::get_material_count()
{
    return 0;
}

void sg_light_object::recalculate_aabb()
{

}

glm::mat4x4 sg_light_object::get_relative_transform()
{
    m_look  = glm::vec3(m_rotation*glm::vec3(0,0,-1));
	m_up    = glm::vec3(m_rotation*glm::vec3(0,1,0));
	m_right = glm::cross(m_look, m_up);

    return glm::lookAt(m_position,m_position+m_look,m_up);
}

}
