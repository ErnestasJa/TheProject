#include "Precomp.h"
#include "SGScenegraph.h"
#include "SGMeshObject.h"
#include "SGGraphicsManager.h"
#include "ISGRenderQueue.h"
#include "opengl/Mesh.h"
#include "opengl/BufferObject.h"
namespace sg
{

sg_mesh_object::sg_mesh_object(SGScenegraph * sg, mesh_ptr ptr): isg_object(sg)
{
    m_mesh = ptr;

    if(ptr)
    {
        recalculate_aabb();

        m_materials.resize(ptr->sub_meshes.size());
        for(uint32_t i=0; i<ptr->sub_meshes.size(); i++)
        {
            m_materials[i]=this->m_scenegraph->get_graphics_manager()
                ->create_material("res/shaders/kursinis/color_mat.vert", "res/shaders/kursinis/color_mat.frag");
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

void sg_mesh_object::render(SGScenegraph * sg)
{
    for(uint32_t i = 0; i < m_mesh->sub_meshes.size(); i++)
    {
        if(i<this->get_material_count())
        {
            sg->get_render_queue()->set_material(this,m_materials[i]);
            m_mesh->render(i);
        }
    }
}

mesh_ptr sg_mesh_object::get_mesh()
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

sg_aabb sg_mesh_object::get_aabb()
{
    return m_aabb;
}

void sg_mesh_object::recalculate_aabb()
{
    if(m_mesh)
    {
        buffer_object<glm::vec3> * bo = static_cast<buffer_object<glm::vec3> *>(m_mesh->buffers[0]);

        if(bo->data.size()>0)
        {
            m_aabb.reset(bo->data[0]);

            for(uint32_t i = 1; i < bo->data.size(); i++)
            {
                m_aabb.add_point(bo->data[i]);
            }
        }
    }
    else
        m_aabb.reset(glm::vec3());
}

}
