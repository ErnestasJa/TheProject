#ifndef SG_LIGHT_OBJECT_H
#define SG_LIGHT_OBJECT_H

#include "isg_object.h"


namespace sg
{

class sg_light_object: public isg_object
{
public:
    sg_light_object(scenegraph * sg);
    virtual ~sg_light_object();

public:
    virtual void            render(scenegraph * sg);

    virtual sg_aabb         get_aabb();
    virtual uint32_t        get_type();

    virtual sg_material_ptr get_material(uint32_t index);
    virtual uint32_t        get_material_count();

    virtual void            recalculate_aabb();

protected:
    sg_aabb m_aabb;
};
typedef std::shared_ptr<sg_light_object> sg_light_object_ptr;
}

#endif // SG_LIGHT_OBJECT_H
