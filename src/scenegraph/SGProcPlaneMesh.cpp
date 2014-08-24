#include "Precomp.h"
#include "SGProcPlaneMesh.h"
#include "SGScenegraph.h"
#include "ISGRenderQueue.h"
#include "opengl/BufferObject.h"

namespace sg
{

SGProcPlaneMesh::SGProcPlaneMesh(SGScenegraph * sg): isg_object(sg)
{
    m_aabb = AABB(glm::vec3(-0.5,-0.5,-0.5),glm::vec3(0.5,0.5,0.5));

    buffer_object<glm::vec3> * pos;
    buffer_object<glm::vec2> * tex_coords;
    index_buffer_object<uint32_t> * indices;

    m_mesh = share(new Mesh());
    pos = new buffer_object<glm::vec3>();

    pos->data.resize(256);

    float scalar=20/16;
    loop(i,16)
    {
        loop(j,16)
        {
            pos->data[16*i+j]=glm::vec3(-10+j*scalar,10-i*scalar,0);
        }
    }

    indices = new index_buffer_object<uint32_t>();
    loop(i,256)
    {
        indices->data.push_back(i);
        indices->data.push_back(i+1);
        indices->data.push_back(i+16);

        indices->data.push_back(i+1);
        indices->data.push_back(i+16+1);
        indices->data.push_back(i+16);
    }

    tex_coords = new buffer_object<glm::vec2>();
//    tex_coords->data.resize(4);
//    tex_coords->data[0] = glm::vec2(0,0);
//    tex_coords->data[1] = glm::vec2(1,0);
//    tex_coords->data[2] = glm::vec2(1,1);
//    tex_coords->data[3] = glm::vec2(0,1);

    m_mesh->buffers[Mesh::BUFFER_OBJECT_INDEX::POSITION] = pos;
    m_mesh->buffers[Mesh::BUFFER_OBJECT_INDEX::TEXCOORD] = tex_coords;
    m_mesh->buffers[Mesh::BUFFER_OBJECT_INDEX::INDICES]  = indices;
    m_mesh->Init();
}

SGProcPlaneMesh::~SGProcPlaneMesh()
{
    //dtor
}

uint32_t SGProcPlaneMesh::get_type()
{
    return SGO_MESH;
}

void SGProcPlaneMesh::Render(SGScenegraph * sg)
{
    sg->get_render_queue()->set_material(this,m_material);
    m_mesh->Render(0);
}

bool SGProcPlaneMesh::set_material(uint32_t index, sg_material_ptr material)
{
    if(index==0)
    {
        m_material = material;
        return true;
    }
    return false;
}

sg_material_ptr SGProcPlaneMesh::get_material(uint32_t index)
{
    return m_material;
}

void SGProcPlaneMesh::set_material(sg_material_ptr mat)
{
    m_material = nullptr;
    m_material = mat;
}

uint32_t SGProcPlaneMesh::get_material_count()
{
    return 1;
}

AABB SGProcPlaneMesh::get_aabb()
{
    return m_aabb;
}

}
