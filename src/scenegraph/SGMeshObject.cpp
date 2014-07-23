#include "Precomp.h"
#include "SGScenegraph.h"
#include "SGMeshObject.h"
#include "SGGraphicsManager.h"
#include "ISGRenderQueue.h"
#include "opengl/Mesh.h"
#include "opengl/BufferObject.h"
namespace sg
{

sg_mesh_object::sg_mesh_object(SGScenegraph * sg, MeshPtr ptr): isg_object(sg)
{
    m_mesh = ptr;

    if(ptr)
    {
        recalculate_aabb();

        m_materials.resize(ptr->sub_meshes.size());
        for(uint32_t i=0; i<ptr->sub_meshes.size(); i++)
        {
            m_materials[i]=this->m_scenegraph->get_graphics_manager()
                ->create_material("res/engine/shaders/solid_unlit_tex.vert", "res/engine/shaders/solid_unlit_tex.frag");
            m_materials[i]->set_int("texture0",0);
        }
    }
}

sg_mesh_object::~sg_mesh_object()
{

}

uint32_t sg_mesh_object::get_type()
{
    return SGO_MESH;
}

void sg_mesh_object::Render(SGScenegraph * sg)
{
    for(uint32_t i = 0; i < m_mesh->sub_meshes.size(); i++)
    {
        if(i<this->get_material_count())
        {
            sg->get_render_queue()->set_material(this,m_materials[i]);
            m_mesh->Render(i);
        }
    }
}

MeshPtr sg_mesh_object::get_mesh()
{
    return m_mesh;
}

bool sg_mesh_object::set_material(uint32_t index, sg_material_ptr mat)
{
    if(index < m_materials.size())
    {
        m_materials[index]=mat;
        return true;
    }
    return false;
}

sg_material_ptr sg_mesh_object::get_material(uint32_t index)
{
    return m_materials[index];
}

uint32_t sg_mesh_object::get_material_count()
{
    return m_materials.size();
}

AABB sg_mesh_object::get_aabb()
{
    return m_mesh->aabb;
}

void sg_mesh_object::recalculate_aabb()
{
    m_mesh->RecalculateAABB();
}

}
