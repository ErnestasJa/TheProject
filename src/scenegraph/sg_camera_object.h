#ifndef SG_CAMERA_OBJECT_H
#define SG_CAMERA_OBJECT_H

#include "isg_object.h"

namespace sg
{

class sg_camera_object: public isg_object
{
public:
    sg_camera_object(const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up, float aspect_ratio=16.0f/9.0f, float field_of_view=45.0f, float near_z=0.1f, float far_z=4096.0f);
    virtual ~sg_camera_object();

    virtual uint32_t get_type();
    virtual void render(scenegraph * sg);
    virtual void on_set_shader_constants(shader_ptr shader);

    virtual sg_material & get_material(uint32_t index);
    virtual uint32_t get_material_count();

    virtual glm::mat4 & get_projection();


    virtual void mouse_moved(double delta_x, double delta_y);
    // FIXME (Ernestas#1#): Need to have some sort of state saving for keyboard keys and execute movement code every frame.
    virtual void key_pressed(int32_t key, int32_t scan_code, int32_t action, int32_t modifier);

protected:
    float m_fov, m_aspect;
    glm::mat4 m_projection;

    glm::vec3 position,target,up;
    glm::vec3 direction, right;
    double horizontalAngle,verticalAngle,speed,mouseSpeed;
    sg_material m_mat;
};

typedef std::shared_ptr<sg_camera_object> sg_camera_object_ptr;

}

#endif // SG_CAMERA_OBJECT_H
