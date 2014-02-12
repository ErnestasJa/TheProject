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
    scenegraph * m_scenegraph;
public:
    isg_object(scenegraph * sg);
    isg_object(const isg_object & sgo)= delete;

    virtual ~isg_object();

    virtual glm::mat4x4 &   get_transform();
    virtual std::string &   get_name();
    virtual sg_aabb         get_aabb();
public:
    virtual uint32_t get_type() = 0;
    virtual void render(scenegraph * sg)=0;

    virtual sg_material_ptr get_material(uint32_t index)=0;
    virtual uint32_t get_material_count()=0;
};

typedef std::shared_ptr<isg_object> sg_object_ptr;
}

#endif // ISG_OBJECT_H
