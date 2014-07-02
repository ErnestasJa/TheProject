#include "precomp.h"
#include "sg_line_object.h"

#include "scenegraph.h"
#include "sg_mesh_object.h"
#include "opengl/mesh.h"
#include "opengl/shader.h"
#include "sg_graphics_manager.h"
#include "isg_render_queue.h"

namespace sg
{

sg_line_object::sg_line_object(scenegraph * sg): isg_object(sg)
{
    m_mesh = mesh_ptr(new mesh());
    m_mesh->buffers[mesh::POSITION] = new buffer_object<glm::vec3>();

    verts = static_cast<buffer_object<glm::vec3> *>(m_mesh->buffers[mesh::POSITION]);

    m_mesh->init();

    m_material = this->m_scenegraph->get_graphics_manager()->create_material(sg::SGM_ABSTRACT_MATERIAL,"res/shaders/kursinis/color_mat.vert","res/shaders/kursinis/color_mat.frag");
    m_abstract_material = static_cast<sg::sg_abstract_material*>(m_material.get());
}

sg_line_object::~sg_line_object()
{
    //dtor
}

uint32_t sg_line_object::get_type()
{
    return SGO_LINE_OBJECT;
}

void sg_line_object::render(scenegraph * sg)
{
    sg->get_render_queue()->set_material(this,m_material);
    m_mesh->render_lines();
}

sg_material_ptr sg_line_object::get_material(uint32_t index)
{
    return m_material;
}

sg_abstract_material * sg_line_object::get_abstract_material()
{
    return m_abstract_material;
}

bool sg_line_object::set_material(uint32_t index, sg_material_ptr mat)
{
    m_material = mat;
    return true;
}

void sg_line_object::add_segment(glm::vec3 p0, glm::vec3 p1)
{
    verts->data.push_back(p0);
    verts->data.push_back(p1);
}

void sg_line_object::clear_segments()
{
    verts->data.clear();
    m_mesh->upload_buffers();
}


void sg_line_object::update_mesh()
{
    m_mesh->upload_buffers();
}

uint32_t sg_line_object::get_material_count()
{
    return 1;
}

}
