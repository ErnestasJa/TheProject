#include "precomp.h"
#include "scenegraph.h"
#include "utility/logger.h"
#include "sg_camera_object.h"

namespace sg
{

sg_camera_object::sg_camera_object(scenegraph * sg, const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up, float aspect_ratio, float field_of_view, float near_z, float far_z): isg_object(sg)
{
    this->set_position(pos);
    this->set_rotation(glm::toQuat(glm::inverse(glm::lookAt(pos,target,up))));

    this->m_look=pos-target;
    this->m_up=up;

    m_fov = field_of_view;
    m_aspect_ratio = aspect_ratio;

    m_P = glm::perspective(field_of_view, aspect_ratio, near_z, far_z);

    this->update_absolute_transform();

    sg->get_logger()->log(LOG_LOG,"m_rotation_quat(%f,%f,%f,%f)", m_rotation.x, m_rotation.y, m_rotation.z, m_rotation.w);
    sg->get_logger()->log(LOG_LOG,"m_look(%f,%f,%f)", m_look.x, m_look.y, m_look.z);
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

bool sg_camera_object::set_material(uint32_t index, sg_material_ptr mat){return false;}
sg_material_ptr sg_camera_object::get_material(uint32_t index){return sg_material_ptr();}
uint32_t sg_camera_object::get_material_count(){return 0;}

glm::mat4 & sg_camera_object::get_projection()
{
    return m_P;
}

const float sg_camera_object::get_fov() const
{
    return m_fov;
}

const float sg_camera_object::get_aspect_ratio() const
{
    return m_aspect_ratio;
}

const glm::vec3 sg_camera_object::get_look() const
{
    return m_look;
}

void sg_camera_object::update(scenegraph * sg)
{
	m_position+=m_translation;
	m_translation=glm::vec3(0);
}

void sg_camera_object::walk(const float amount) {
	m_translation += (m_look*amount);
    m_flags |= SGOF_TRANSFORM_OUTDATED;
}

void sg_camera_object::strafe(const float amount) {
	m_translation += (m_right*amount);
    m_flags |= SGOF_TRANSFORM_OUTDATED;
}

void sg_camera_object::lift(const float amount) {
	m_translation += (m_up*amount);
    m_flags |= SGOF_TRANSFORM_OUTDATED;
}

glm::mat4x4 sg_camera_object::get_relative_transform()
{
    //return isg_object::get_relative_transform();
    m_look  = glm::vec3(m_rotation*glm::vec3(0,0,-1));
	m_up    = glm::vec3(m_rotation*glm::vec3(0,1,0));

	m_right = glm::cross(m_look, m_up);

    return glm::lookAt(m_position,m_position+m_look,m_up);
}

}
