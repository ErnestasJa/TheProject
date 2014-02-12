#include "precomp.h"
#include "scenegraph.h"
#include "sg_mesh_object.h"
#include "sg_graphics_manager.h"
#include "opengl/mesh.h"
#include "opengl/buffer_object.h"
namespace sg
{

sg_mesh_object::sg_mesh_object(scenegraph * sg, mesh_ptr ptr): isg_object(sg)
{
    m_mesh = ptr;

    if(ptr)
    {
        recalculate_aabb();

        m_materials.resize(ptr->sub_meshes.size());
        loopi(ptr->sub_meshes.size())
        {
            m_materials[i]=std::static_pointer_cast<sg_material_static_mesh>(this->m_scenegraph->get_graphics_manager()->create_material(SGMT_STATIC_MESH));
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

void sg_mesh_object::render(scenegraph * sg)
{
    for(uint32_t i = 0; i < m_mesh->sub_meshes.size(); i++)
    {
        if(i<this->get_material_count())
        {
            sg_material_static_mesh * mat = m_materials[i].get();
            mat->m = this->get_transform();
            mat->mv = sg->get_active_camera()->get_transform()*this->get_transform();
            mat->mvp = sg->get_active_camera()->get_projection() * mat->mv.value;
            mat->n = glm::inverseTranspose(glm::mat3(mat->m.value));
            mat->light_pos = glm::vec3(0,20,100);
            mat->camera_pos = sg->get_active_camera()->get_position();

            sg->on_set_material(m_materials[i]);

            m_mesh->render(i);
        }
    }
}

mesh_ptr sg_mesh_object::get_mesh()
{
    return m_mesh;
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
