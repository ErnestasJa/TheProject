#include "precomp.h"
#include "sg_camera_object.h"

namespace sg
{

sg_camera_object::sg_camera_object(scenegraph * sg, const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up, float aspect_ratio, float field_of_view, float near_z, float far_z): isg_object(sg)
{
    this->m_position=pos;
    m_P = glm::perspective(field_of_view, aspect_ratio, near_z, far_z);
    m_fov = field_of_view;
    m_aspect_ratio = aspect_ratio;
}

sg_camera_object::~sg_camera_object()
{
}

uint32_t sg_camera_object::get_type()
{
    return SGO_CAMERA;
}

void            sg_camera_object::render(scenegraph * sg){};
void            sg_camera_object::on_set_shader_constants(shader_ptr shader){};
sg_material_ptr   sg_camera_object::get_material(uint32_t index){return sg_material_ptr();}
uint32_t        sg_camera_object::get_material_count(){return 1;}

glm::mat4 & sg_camera_object::get_projection()
{
    return m_P;
}

glm::mat4 sg_camera_object::get_matrix(const float yaw, const float pitch, const float roll)
{
    glm::mat4 R=glm::mat4(1);

	R = glm::rotate(R, roll, glm::vec3(0,0,1));
	R = glm::rotate(R,  yaw, glm::vec3(0,1,0));
	R = glm::rotate(R, pitch, glm::vec3(1,0,0));

	return R;
}

const float sg_camera_object::get_fov() const
{
    return m_fov;
}

const float sg_camera_object::get_aspect_ratio() const
{
    return m_aspect_ratio;
}

const glm::vec3 sg_camera_object::get_position() const
{
    return m_position;
}

const glm::vec3 sg_camera_object::get_look() const
{
    return m_look;
}

void sg_camera_object::update(scenegraph * sg)
{
    glm::mat4 R = get_matrix(m_yaw,m_pitch,m_roll);
	m_position+=m_translation;
	m_translation=glm::vec3(0);

	m_look = glm::vec3(R*glm::vec4(0,0,1,0));
	glm::vec3 tgt  = m_position+m_look;
	m_up   = glm::vec3(R*glm::vec4(0,1,0,0));
	m_right = glm::cross(m_look, m_up);
	m_transform = glm::lookAt(m_position, tgt, m_up);
}


void sg_camera_object::rotate(const float y, const float p, const float r) {
	m_yaw=y;
	m_pitch=p;
	m_roll=r;
}

void sg_camera_object::walk(const float amount) {
	m_translation += (m_look*amount);
}

void sg_camera_object::strafe(const float amount) {
	m_translation += (m_right*amount);
}

void sg_camera_object::lift(const float amount) {
	m_translation += (m_up*amount);
}

}
