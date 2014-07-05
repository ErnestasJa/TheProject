#ifndef SG_CAMERA_OBJECT_H
#define SG_CAMERA_OBJECT_H

#include "ISGObject.h"

struct AppContext;
namespace sg
{

class sg_camera_object: public isg_object
{
public:
    sg_camera_object(AppContext* ctx, const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up, float aspect_ratio=1.777777f, float field_of_view=45.0f, float near_z=1.0f, float far_z=4096.0f);
    virtual ~sg_camera_object();

    virtual uint32_t get_type();
    virtual void render(SGScenegraph * sg);
    virtual void on_set_shader_constants(shader_ptr shader);

    virtual bool set_material(uint32_t index, sg_material_ptr mat);
    virtual sg_material_ptr get_material(uint32_t index);
    virtual uint32_t get_material_count();

    virtual glm::mat4 & get_projection();

	const glm::vec3 get_look() const;

	const float get_far() const;
	const float get_near() const;
	const float get_fov() const;
	const float get_aspect_ratio() const;

    virtual void update(SGScenegraph * sg);

public:
	void walk(const float amount);
	void strafe(const float amount);
	void lift(const float amount);
	virtual glm::mat4x4 get_relative_transform();
	void orbit(glm::vec3 point,float distance,float angleX,float angleY);
	void handle_mouse(int x,int y);

protected:
    float m_fov, m_aspect_ratio, m_far, m_near;
	static glm::vec3 UP;
	glm::vec3 m_look;
	glm::vec3 m_up;
	glm::vec3 m_right;
	glm::mat4 m_P; //projection matrix

	glm::ivec2 m_last_mouse_pos,m_current_mouse_pos;

	glm::vec3 m_translation;

	sg_material_ptr m_mat;

	bool m_fps;
	AppContext* _appContext;
};

typedef std::shared_ptr<sg_camera_object> sg_camera_object_ptr;

}

#endif // SG_CAMERA_OBJECT_H
