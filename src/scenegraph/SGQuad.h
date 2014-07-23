#ifndef SG_QUAD_H
#define SG_QUAD_H

#include "opengl/Mesh.h"
#include "ISGObject.h"
#include "SGMaterial.h"
#include "resources/ResourceCache.h"

namespace sg
{

class sg_quad: public isg_object
{
public:
    sg_quad(SGScenegraph * sg);
    virtual ~sg_quad();

    virtual void Render(SGScenegraph * sg);

    virtual uint32_t get_type();
    virtual bool set_material(uint32_t index, sg_material_ptr material);
    virtual sg_material_ptr get_material(uint32_t index);
    virtual void set_material(sg_material_ptr mat);
    virtual uint32_t get_material_count();
    virtual AABB get_aabb();

protected:
    AABB     m_aabb;
    MeshPtr    m_mesh;
    sg_material_ptr m_material;

};

typedef std::shared_ptr<sg_quad> sg_quad_ptr;

}

#endif // SG_QUAD_H
