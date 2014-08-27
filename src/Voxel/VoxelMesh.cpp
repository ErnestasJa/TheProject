#include "precomp.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"

#include "VoxelMesh.h"

VoxelMesh::VoxelMesh()
{
    m_dirty=false;
    m_indexTrack=0;

    m_colBuf=new BufferObject<glm::vec4>();
    m_posBuf=new BufferObject<glm::vec3>();
    m_indBuf=new IndexBufferObject<uint32_t>();

    m_mesh = share(new Mesh());

    m_mesh->buffers[Mesh::POSITION] = m_posBuf;
    m_mesh->buffers[Mesh::COLOR] = m_colBuf;
    m_mesh->buffers[Mesh::INDICES] = m_indBuf;

    m_mesh->Init();
}

VoxelMesh::~VoxelMesh()
{
    delete m_colBuf;
    delete m_posBuf;
    delete m_indBuf;
}

void VoxelMesh::Render()
{
    if(m_dirty)
        Rebuild();
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    m_mesh->Render();
    //glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
}

void VoxelMesh::Rebuild()
{
    m_mesh->UploadBuffers();
    m_dirty=false;
}

void VoxelMesh::Cleanup()
{
    m_posBuf->data.clear();
    m_indBuf->data.clear();
    m_colBuf->data.clear();

    m_mesh->buffers[Mesh::POSITION] = m_posBuf;
    m_mesh->buffers[Mesh::COLOR] = m_colBuf;
    m_mesh->buffers[Mesh::INDICES] = m_indBuf;
}

void VoxelMesh::CreateVoxel(float x, float y, float z, uint32_t sides, glm::vec4 color)
{
    float BLOCK_RENDER_SIZE=0.5;
    /// - - +
    glm::vec3 p1(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
    /// + - +
    glm::vec3 p2(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
    /// + + +
    glm::vec3 p3(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
    /// - + +
    glm::vec3 p4(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);

    /// - - -
    glm::vec3 p5(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
    /// + - -
    glm::vec3 p6(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
    /// + + -
    glm::vec3 p7(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
    /// - + -
    glm::vec3 p8(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);

    m_posBuf->data.push_back(p1);
    m_posBuf->data.push_back(p2);
    m_posBuf->data.push_back(p3);
    m_posBuf->data.push_back(p4);
    m_posBuf->data.push_back(p5);
    m_posBuf->data.push_back(p6);
    m_posBuf->data.push_back(p7);
    m_posBuf->data.push_back(p8);

    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);
    m_colBuf->data.push_back(color);

    // Front
    if(CheckBit(sides,EBS_FRONT))
    {
        m_indBuf->data.push_back(m_indexTrack+0);
        m_indBuf->data.push_back(m_indexTrack+1);
        m_indBuf->data.push_back(m_indexTrack+2);

        m_indBuf->data.push_back(m_indexTrack+2);
        m_indBuf->data.push_back(m_indexTrack+3);
        m_indBuf->data.push_back(m_indexTrack+0);
    }

    // Top
    if(CheckBit(sides,EBS_TOP))
    {
        m_indBuf->data.push_back(m_indexTrack+3);
        m_indBuf->data.push_back(m_indexTrack+2);
        m_indBuf->data.push_back(m_indexTrack+6);

        m_indBuf->data.push_back(m_indexTrack+6);
        m_indBuf->data.push_back(m_indexTrack+7);
        m_indBuf->data.push_back(m_indexTrack+3);
    }

    // Back
    if(CheckBit(sides,EBS_BACK))
    {
        m_indBuf->data.push_back(m_indexTrack+7);
        m_indBuf->data.push_back(m_indexTrack+6);
        m_indBuf->data.push_back(m_indexTrack+5);

        m_indBuf->data.push_back(m_indexTrack+5);
        m_indBuf->data.push_back(m_indexTrack+4);
        m_indBuf->data.push_back(m_indexTrack+7);
    }

    // Bottom
    if(CheckBit(sides,EBS_BOTTOM))
    {
        m_indBuf->data.push_back(m_indexTrack+4);
        m_indBuf->data.push_back(m_indexTrack+5);
        m_indBuf->data.push_back(m_indexTrack+1);

        m_indBuf->data.push_back(m_indexTrack+1);
        m_indBuf->data.push_back(m_indexTrack+0);
        m_indBuf->data.push_back(m_indexTrack+4);
    }

    // Left
    if(CheckBit(sides,EBS_LEFT))
    {
        m_indBuf->data.push_back(m_indexTrack+4);
        m_indBuf->data.push_back(m_indexTrack+0);
        m_indBuf->data.push_back(m_indexTrack+3);

        m_indBuf->data.push_back(m_indexTrack+3);
        m_indBuf->data.push_back(m_indexTrack+7);
        m_indBuf->data.push_back(m_indexTrack+4);
    }

    // Right
    if(CheckBit(sides,EBS_RIGHT))
    {
        m_indBuf->data.push_back(m_indexTrack+1);
        m_indBuf->data.push_back(m_indexTrack+5);
        m_indBuf->data.push_back(m_indexTrack+6);

        m_indBuf->data.push_back(m_indexTrack+6);
        m_indBuf->data.push_back(m_indexTrack+2);
        m_indBuf->data.push_back(m_indexTrack+1);
    }
    m_dirty=true;
    m_indexTrack+=8;
}
