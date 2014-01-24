#ifndef SG_CAMERA_OBJECT_H
#define SG_CAMERA_OBJECT_H

#include "isg_object.h"

namespace sg
{

class sg_camera_object: public isg_object
{
public:
    sg_camera_object(float aspect_ratio, float field_of_view, float near_z, float far_z);
    virtual ~sg_camera_object();

    virtual uint32_t get_type();
    virtual void render(scenegraph * sg);
    virtual void on_set_shader_constants(shader_ptr shader);

    virtual sg_material & get_material(uint32_t index);
    virtual uint32_t get_material_count();

    virtual glm::mat4 & get_projection();

protected:

    float m_fov, m_aspect;
    glm::mat4 m_projection;
    sg_material m_mat;
};

typedef std::shared_ptr<sg_camera_object> sg_camera_object_ptr;

}

#endif // SG_CAMERA_OBJECT_H
