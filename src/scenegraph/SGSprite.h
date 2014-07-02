#ifndef SG_SPRITE_H
#define SG_SPRITE_H

#include "ISGObject.h"

class mesh;
typedef std::shared_ptr<mesh> mesh_ptr;

namespace sg
{

class sg_sprite: public isg_object
{
public:
    sg_sprite(scenegraph * sg);
    virtual ~sg_sprite();

public:

    virtual void            render(scenegraph * sg);
    virtual sg_aabb         get_aabb();
    virtual uint32_t        get_type();

    virtual sg_material_ptr get_material(uint32_t index);
    virtual uint32_t        get_material_count();

    virtual void            recalculate_aabb();

protected:
    sg_aabb m_aabb;
    mesh_ptr m_mesh;
    sg_material_point_sprite_ptr m_material;
};
typedef std::shared_ptr<sg_sprite> sg_sprite_ptr;
}

#endif // SG_SPRITE_H
