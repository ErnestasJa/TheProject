#include "precomp.h"
#include "sg_camera_object.h"

namespace sg
{

sg_camera_object::sg_camera_object(float aspect_ratio, float field_of_view, float near_z, float far_z)
{
    m_projection = glm::perspective(field_of_view, aspect_ratio, near_z, far_z);
}

sg_camera_object::~sg_camera_object()
{
}

uint32_t sg_camera_object::get_type()
{
    return SGO_CAMERA;
}

void sg_camera_object::render(scenegraph * sg){};
void sg_camera_object::on_set_shader_constants(shader_ptr shader){};

sg_material & sg_camera_object::get_material(uint32_t index){return m_mat;}
uint32_t sg_camera_object::get_material_count(){return 1;}

glm::mat4 & sg_camera_object::get_projection()
{
    return m_projection;
}

}
