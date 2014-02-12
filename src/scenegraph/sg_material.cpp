#include "precomp.h"
#include "sg_material.h"
#include "scenegraph.h"
#include "opengl/shader.h"

namespace sg
{

sg_material::sg_material()
{
}

sg_material::~sg_material()
{
}

///----------------------------------------------------------

sg_material_static_mesh::sg_material_static_mesh()
{
    mat_type = SGMT_STATIC_MESH;

    m.name="M";
    mv.name="MV";
    mvp.name="MVP";
    n.name="N";
    light_pos.name="light_pos";
    camera_pos.name="camera_pos";
}

void sg_material_static_mesh::set(scenegraph * sg)
{
    mat_shader->set();

    mat_texture->set(0);
    m.set(mat_shader);
    mv.set(mat_shader);
    mvp.set(mat_shader);
    n.set(mat_shader);
    light_pos.set(mat_shader);
    camera_pos.set(mat_shader);
}

}
