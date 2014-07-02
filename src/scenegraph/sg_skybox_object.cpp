#include "precomp.h"
#include "scenegraph.h"
#include "sg_skybox_object.h"
#include "sg_graphics_manager.h"
#include "isg_render_queue.h"
#include "opengl/mesh.h"
#include "opengl/buffer_object.h"

namespace sg
{

sg_skybox_object::sg_skybox_object(scenegraph * sg, mesh_ptr ptr): isg_object(sg)
{
    m_mesh = ptr;

    if(ptr)
    {
        recalculate_aabb();

        m_materials.resize(ptr->sub_meshes.size());
        for(uint32_t i=0; i<ptr->sub_meshes.size(); i++)
        {
            m_materials[i]=this->m_scenegraph->get_graphics_manager()->create_material(SGM_ABSTRACT_MATERIAL,"res/shaders/skybox/skybox.vert","res/shaders/skybox/skybox.frag");
            sg_abstract_material * am = (sg_abstract_material *)m_materials[i].get();
            am->set_int("texture0",0);
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

void sg_skybox_object::render(scenegraph * sg)
{
    glDepthMask(0);

    for(uint32_t i = 0; i < m_mesh->sub_meshes.size(); i++)
    {
        if(i<this->get_material_count())
        {
            sg->get_render_queue()->set_material(this,m_materials[i]);
            m_mesh->render(i);
        }
    }

    glDepthMask(1);
}

mesh_ptr sg_skybox_object::get_mesh()
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

sg_aabb sg_skybox_object::get_aabb()
{
    return m_aabb;
}

void sg_skybox_object::recalculate_aabb()
{
    if(m_mesh)
    {
        buffer_object<glm::vec3> * bo = static_cast<buffer_object<glm::vec3> *>(m_mesh->buffers[mesh::POSITION]);

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