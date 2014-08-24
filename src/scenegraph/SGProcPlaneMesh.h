#ifndef SGPROCPLANEMESH_H
#define SGPROCPLANEMESH_H

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"

namespace sg
{

class SGProcPlaneMesh: public isg_object
{
public:
    SGProcPlaneMesh(SGScenegraph * sg);
    virtual ~SGProcPlaneMesh();

    virtual void Render(SGScenegraph * sg);

    virtual uint32_t get_type();
    virtual bool set_material(uint32_t index, sg_material_ptr material);
    virtual sg_material_ptr get_material(uint32_t index);
    virtual void set_material(sg_material_ptr mat);
    virtual uint32_t get_material_count();
    virtual AABB get_aabb();
    MeshPtr get_mesh()
    {
        return m_mesh;
    }

protected:
    AABB     m_aabb;
    MeshPtr    m_mesh;
    sg_material_ptr m_material;

};

typedef std::shared_ptr<SGProcPlaneMesh> SGProcPlaneMeshPtr;

}


#endif // SGPROCPLANEMESH_H
