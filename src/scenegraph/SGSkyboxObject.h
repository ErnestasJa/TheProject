#ifndef SG_SKYBOX_OBJECT_H
#define SG_SKYBOX_OBJECT_H

#include "ISGObject.h"

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

namespace sg
{

class sg_skybox_object: public isg_object
{
public:
    sg_skybox_object(SGScenegraph * sg, MeshPtr ptr);
    virtual ~sg_skybox_object();

    virtual MeshPtr get_mesh();

public:

    virtual void Render(SGScenegraph * sg);

    virtual uint32_t get_type();

    virtual bool set_material(uint32_t index, sg_material_ptr mat);
    virtual sg_material_ptr get_material(uint32_t index);
    virtual uint32_t get_material_count();

    virtual AABB GetAABB();
    virtual void recalculate_aabb();

protected:
    AABB m_aabb;
    MeshPtr m_mesh;
    std::vector<sg_material_ptr> m_materials;
};

typedef std::shared_ptr<sg_skybox_object> sg_skybox_object_ptr;

}

#endif // SG_SKYBOX_OBJECT_H
