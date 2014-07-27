#include "Precomp.h"
#include "SGScenegraph.h"
#include "SGSkyboxObject.h"
#include "SGGraphicsManager.h"
#include "ISGRenderQueue.h"
#include "opengl/Mesh.h"
#include "opengl/BufferObject.h"

namespace sg
{

sg_skybox_object::sg_skybox_object(SGScenegraph * sg, MeshPtr ptr): isg_object(sg)
{
    m_mesh = ptr;

    if(ptr)
    {
        recalculate_aabb();

        m_materials.resize(ptr->sub_meshes.size());
        for(uint32_t i=0; i<ptr->sub_meshes.size(); i++)
        {
            m_materials[i]=this->m_scenegraph->get_graphics_manager()->create_material("res/shaders/skybox/skybox.vert","res/shaders/skybox/skybox.frag");
            m_materials[i]->set_int("texture0",0);
        }
    }
}

sg_skybox_object::~sg_skybox_object()
{

}

uint32_t sg_skybox_object::get_type()
{
    return SGO_SKYBOX_OBJECT;
}

void sg_skybox_object::Render(SGScenegraph * sg)
{
    glDepthMask(0);

    for(uint32_t i = 0; i < m_mesh->sub_meshes.size(); i++)
    {
        if(i<this->get_material_count())
        {
            sg->get_render_queue()->set_material(this,m_materials[i]);
            m_mesh->Render(i);
        }
    }

    glDepthMask(1);
}

MeshPtr sg_skybox_object::get_mesh()
{
    return m_mesh;
}

bool sg_skybox_object::set_material(uint32_t index, sg_material_ptr mat)
{
    if(index < m_materials.size())
    {
        m_materials[index]=mat;
        return true;
    }
    return false;
}

sg_material_ptr sg_skybox_object::get_material(uint32_t index)
{
    return m_materials[index];
}

uint32_t sg_skybox_object::get_material_count()
{
    return m_materials.size();
}

AABB sg_skybox_object::GetAABB()
{
    return m_aabb;
}

void sg_skybox_object::recalculate_aabb()
{
    if(m_mesh)
    {
        buffer_object<glm::vec3> * bo = static_cast<buffer_object<glm::vec3> *>(m_mesh->buffers[Mesh::POSITION]);

        if(bo->data.size()>0)
        {
            m_aabb.Reset(bo->data[0]);

            for(uint32_t i = 1; i < bo->data.size(); i++)
            {
                m_aabb.AddPoint(bo->data[i]);
            }
        }
    }
    else
        m_aabb.Reset(glm::vec3());
}

}
