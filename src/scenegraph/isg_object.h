#ifndef ISG_OBJECT_H
#define ISG_OBJECT_H

#include "sg_object_types.h"
#include "sg_material.h"
#include "sg_aabb.h"

namespace sg
{
class scenegraph;
class isg_object
{
protected:
    glm::mat4x4 m_transform;
    std::string m_name;

public:
    isg_object();
    virtual ~isg_object();

    virtual glm::mat4x4 & get_transform();
    virtual std::string & get_name();

public:
    virtual uint32_t get_type() = 0;
    virtual void render(scenegraph * sg)=0;
    virtual void on_set_shader_constants(shader_ptr shader)=0;

    virtual sg_material & get_material(uint32_t index)=0;
    virtual uint32_t get_material_count()=0;

    virtual sg_aabb get_aabb();
};

typedef std::shared_ptr<isg_object> sg_object_ptr;
}

#endif // ISG_OBJECT_H
