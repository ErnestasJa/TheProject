#include "precomp.h"
#include "sg_quad.h"
#include "scenegraph.h"
#include "isg_render_queue.h"
#include "opengl/buffer_object.h"

namespace sg
{

sg_quad::sg_quad(scenegraph * sg): isg_object(sg)
{
    m_aabb = sg_aabb(glm::vec3(-0.5,-0.5,-0.5),glm::vec3(0.5,0.5,0.5));

    buffer_object<glm::vec3> * pos;
    buffer_object<glm::vec2> * tex_coords;
    index_buffer_object<uint32_t> * indices;

    m_mesh = share(new mesh());
    pos = new buffer_object<glm::vec3>();
    pos->data.resize(4);
    pos->data[0]=glm::vec3(-1,-1,0);
    pos->data[1]=glm::vec3(1,-1,0);
    pos->data[2]=glm::vec3(1,1,0);
    pos->data[3]=glm::vec3(-1,1,0);

    tex_coords = new buffer_object<glm::vec2>();
    tex_coords->data.resize(4);
    tex_coords->data[0] = glm::vec2(0,0);
    tex_coords->data[1] = glm::vec2(1,0);
    tex_coords->data[2] = glm::vec2(1,1);
    tex_coords->data[3] = glm::vec2(0,1);

    indices = new index_buffer_object<uint32_t>();
    indices->data.resize(6);
    indices->data[0]=0;
    indices->data[1]=1;
    indices->data[2]=3;

    indices->data[3]=3;
    indices->data[4]=1;
    indices->data[5]=2;

    m_mesh->buffers[mesh::BUFFER_OBJECT_INDEX::POSITION] = pos;
    m_mesh->buffers[mesh::BUFFER_OBJECT_INDEX::TEXCOORD] = tex_coords;
    m_mesh->buffers[mesh::BUFFER_OBJECT_INDEX::INDICES]  = indices;
    m_mesh->init();
}

sg_quad::~sg_quad()
{
    //dtor
}

uint32_t sg_quad::get_type()
{
    return SGO_QUAD;
}

void sg_quad::render(scenegraph * sg)
{
    sg->get_render_queue()->set_material(this,m_material);
    m_mesh->render(0);
}

sg_material_ptr sg_quad::get_material(uint32_t index)
{
    return m_material;
}

void sg_quad::set_material(sg_material_ptr mat)
{
    m_material = mat;
}

uint32_t sg_quad::get_material_count()
{
    return 1;
}

sg_aabb sg_quad::get_aabb()
{
    return m_aabb;
}

}
