#ifndef SG_CAMERA_OBJECT_H
#define SG_CAMERA_OBJECT_H

#include "isg_object.h"

namespace sg
{

class sg_camera_object: public isg_object
{
public:
    sg_camera_object(scenegraph * sg, const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up, float aspect_ratio=1.777777f, float field_of_view=45.0f, float near_z=1.0f, float far_z=4096.0f);
    virtual ~sg_camera_object();

    virtual uint32_t get_type();
    virtual void render(scenegraph * sg);
    virtual void on_set_shader_constants(shader_ptr shader);

    virtual sg_material_ptr get_material(uint32_t index);
    virtual uint32_t get_material_count();

    virtual glm::mat4 & get_projection();

    void set_position(const glm::vec3 v);
	const glm::vec3 get_position() const;
	const glm::vec3 get_look() const;

	glm::mat4 get_matrix(const float yaw, const float pitch, const float roll);

	const float get_fov() const;
	const float get_aspect_ratio() const;

    virtual void update(scenegraph * sg);

public:
	void walk(const float amount);
	void strafe(const float amount);
	void lift(const float amount);

protected:
    float m_fov, m_aspect_ratio;
	static glm::vec3 UP;
	glm::vec3 m_look;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::mat4 m_P; //projection matrix

	float m_yaw, m_pitch, m_roll;
	glm::vec3 m_translation;

	sg_material_ptr m_mat;
};

typedef std::shared_ptr<sg_camera_object> sg_camera_object_ptr;

}

#endif // SG_CAMERA_OBJECT_H
