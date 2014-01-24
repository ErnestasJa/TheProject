#include "precomp.h"
#include "scenegraph.h"
#include "sg_mesh_object.h"
#include "opengl/mesh.h"
namespace sg
{

sg_mesh_object::sg_mesh_object(mesh_ptr ptr)
{
    m_mesh = ptr;

    if(ptr)
        m_materials.resize(ptr->sub_meshes.size());
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

}
