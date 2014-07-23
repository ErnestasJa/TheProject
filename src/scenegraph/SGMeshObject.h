#ifndef SG_MESH_OBJECT_H
#define SG_MESH_OBJECT_H

#include "ISGObject.h"

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

namespace sg
{

class sg_mesh_object: public isg_object
{
public:
    sg_mesh_object(SGScenegraph * sg, MeshPtr ptr);
    virtual ~sg_mesh_object();

    virtual MeshPtr get_mesh();

public:

    virtual void Render(SGScenegraph * sg);

    virtual uint32_t get_type();

    virtual bool set_material(uint32_t index, sg_material_ptr mat);
    virtual sg_material_ptr get_material(uint32_t index);
    virtual uint32_t get_material_count();

    virtual AABB get_aabb();
    virtual void recalculate_aabb();

protected:
    MeshPtr m_mesh;
    std::vector<sg_material_ptr> m_materials;
};

typedef std::shared_ptr<sg_mesh_object> sg_mesh_object_ptr;

}

#endif // SG_MESH_OBJECT_H
