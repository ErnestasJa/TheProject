#include "precomp.h"
#include "scenegraph.h"
#include "sg_mesh_object.h"
#include "opengl/mesh.h"
#include "opengl/buffer_object.h"
namespace sg
{

sg_mesh_object::sg_mesh_object(mesh_ptr ptr)
{
    m_mesh = ptr;

    if(ptr)
    {
        m_materials.resize(ptr->sub_meshes.size());
        recalculate_aabb();
    }
}

sg_mesh_object::~sg_mesh_object()
{

}

uint32_t sg_mesh_object::get_type()
{
    return SGO_MESH;
}

void sg_mesh_object::render(scenegraph * sg)
{
    for(uint32_t i = 0; i < m_mesh->sub_meshes.size(); i++)
    {
        if(i<this->get_material_count())
        {
            sg->on_set_material(this->get_material(i));
            m_mesh->render(i);
        }
    }
}

void sg_mesh_object::on_set_shader_constants(shader_ptr shader)
{

}

mesh_ptr sg_mesh_object::get_mesh()
{
    return m_mesh;
}

sg_material & sg_mesh_object::get_material(uint32_t index)
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
