#ifndef SG_MESH_OBJECT_H
#define SG_MESH_OBJECT_H

#include "isg_object.h"

class mesh;
typedef std::shared_ptr<mesh> mesh_ptr;

namespace sg
{

class sg_mesh_object: public isg_object
{
public:
    sg_mesh_object(mesh_ptr ptr);
    virtual ~sg_mesh_object();

    virtual mesh_ptr get_mesh();

public:
    virtual uint32_t get_type();

    virtual void render(scenegraph * sg);
    virtual void on_set_shader_constants(shader_ptr shader);

    virtual sg_material & get_material(uint32_t index);
    virtual uint32_t get_material_count();

    virtual sg_aabb get_aabb();

    virtual void recalculate_aabb();

protected:
    sg_aabb m_aabb;
    mesh_ptr m_mesh;
    std::vector<sg_material> m_materials;
};

typedef std::shared_ptr<sg_mesh_object> sg_mesh_object_ptr;

}

#endif // SG_MESH_OBJECT_H
