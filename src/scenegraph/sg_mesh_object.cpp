#include "precomp.h"
#include "sg_mesh_object.h"
#include "opengl/mesh.h"
namespace sg
{

sg_mesh_object::sg_mesh_object(mesh_ptr ptr)
{
    m_mesh = ptr;
}

sg_mesh_object::~sg_mesh_object()
{

}

uint32_t sg_mesh_object::get_type()
{
    return SGO_MESH;
}

void sg_mesh_object::render()
{
    m_mesh->render();
}

}
