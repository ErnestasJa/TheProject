#ifndef SG_LIGHT_OBJECT_H
#define SG_LIGHT_OBJECT_H

#include "ISGObject.h"


namespace sg
{

class sg_light_object: public isg_object
{
public:
    sg_light_object(SGScenegraph * sg);
    virtual ~sg_light_object();

public:
    virtual void            render(SGScenegraph * sg);

    virtual sg_aabb         get_aabb();
    virtual uint32_t        get_type();

    virtual sg_material_ptr get_material(uint32_t index);
    virtual bool set_material(uint32_t index, sg_material_ptr mat);
    virtual uint32_t        get_material_count();

    virtual void            recalculate_aabb();
    virtual glm::mat4x4     get_relative_transform();

protected:
    sg_aabb m_aabb;
    glm::vec3 m_look;
	glm::vec3 m_up;
	glm::vec3 m_right;
};
typedef std::shared_ptr<sg_light_object> sg_light_object_ptr;
}

#endif // SG_LIGHT_OBJECT_H
