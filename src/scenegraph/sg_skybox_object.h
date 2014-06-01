#ifndef SG_SKYBOX_OBJECT_H
#define SG_SKYBOX_OBJECT_H

#include "isg_object.h"

class mesh;
typedef std::shared_ptr<mesh> mesh_ptr;

namespace sg
{

class sg_skybox_object: public isg_object
{
public:
    sg_skybox_object(scenegraph * sg, mesh_ptr ptr);
    virtual ~sg_skybox_object();

    virtual mesh_ptr get_mesh();

public:

    virtual void render(scenegraph * sg);

    virtual uint32_t get_type();

    virtual bool set_material(uint32_t index, sg_material_ptr mat);
    virtual sg_material_ptr get_material(uint32_t index);
    virtual uint32_t get_material_count();

    virtual sg_aabb get_aabb();
    virtual void recalculate_aabb();

protected:
    sg_aabb m_aabb;
    mesh_ptr m_mesh;
    std::vector<sg_material_ptr> m_materials;
};

typedef std::shared_ptr<sg_skybox_object> sg_skybox_object_ptr;

}

#endif // SG_SKYBOX_OBJECT_H
