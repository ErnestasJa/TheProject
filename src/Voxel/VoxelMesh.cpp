#include "precomp.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"
#include "Chunk.h"
#include "VoxelMesh.h"

VoxelMesh::VoxelMesh()
{
    m_dirty=false;
    m_indexTrack=0;
    m_vertexTrack=0;

    m_posBuf=new BufferObject<u8vec3>();
    m_colBuf=new BufferObject<u8vec4>();
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

bool VoxelMesh::Empty()
{
    return m_vertexTrack==0;
}

void VoxelMesh::Render()
{
    if(m_dirty)
        Rebuild();

    if(!Empty()&&!m_dirty)
        m_mesh->Render();

}

void VoxelMesh::Cleanup()
{
    if(!Empty())
    {
        m_posBuf->data.clear();
        m_colBuf->data.clear();
        m_indBuf->data.clear();
        m_indexTrack=0;
        m_vertexTrack=0;
    }
}

void VoxelMesh::UpdateMesh()
{
    if(!Empty())
    {
        m_mesh->buffers[Mesh::POSITION] = m_posBuf;
        m_mesh->buffers[Mesh::COLOR] = m_colBuf;
        m_mesh->buffers[Mesh::INDICES] = m_indBuf;

        m_mesh->UploadBuffers();
    }
}

void VoxelMesh::CreateVoxel(uint8_t x, uint8_t y, uint8_t z, uint32_t sides, u8vec4 color)
{
    uint8_t BLOCK_RENDER_SIZE=1;
    /// - - +
    u8vec3 p1(x, y, z+BLOCK_RENDER_SIZE);
    /// + - +
    u8vec3 p2(x+BLOCK_RENDER_SIZE, y, z+BLOCK_RENDER_SIZE);
    /// + + +
    u8vec3 p3(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
    /// - + +
    u8vec3 p4(x, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);

    /// - - -
    u8vec3 p5(x, y, z);
    /// + - -
    u8vec3 p6(x+BLOCK_RENDER_SIZE, y, z);
    /// + + -
    u8vec3 p7(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z);
    /// - + -
    u8vec3 p8(x, y+BLOCK_RENDER_SIZE, z);

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
        m_indBuf->data.push_back(m_vertexTrack+0);
        m_indBuf->data.push_back(m_vertexTrack+1);
        m_indBuf->data.push_back(m_vertexTrack+2);

        m_indBuf->data.push_back(m_vertexTrack+2);
        m_indBuf->data.push_back(m_vertexTrack+3);
        m_indBuf->data.push_back(m_vertexTrack+0);

        m_indexTrack+=6;
    }

    // Top
    if(CheckBit(sides,EBS_TOP))
    {
        m_indBuf->data.push_back(m_vertexTrack+3);
        m_indBuf->data.push_back(m_vertexTrack+2);
        m_indBuf->data.push_back(m_vertexTrack+6);

        m_indBuf->data.push_back(m_vertexTrack+6);
        m_indBuf->data.push_back(m_vertexTrack+7);
        m_indBuf->data.push_back(m_vertexTrack+3);

        m_indexTrack+=6;
    }

    // Back
    if(CheckBit(sides,EBS_BACK))
    {
        m_indBuf->data.push_back(m_vertexTrack+7);
        m_indBuf->data.push_back(m_vertexTrack+6);
        m_indBuf->data.push_back(m_vertexTrack+5);

        m_indBuf->data.push_back(m_vertexTrack+5);
        m_indBuf->data.push_back(m_vertexTrack+4);
        m_indBuf->data.push_back(m_vertexTrack+7);

        m_indexTrack+=6;
    }

    // Bottom
    if(CheckBit(sides,EBS_BOTTOM))
    {
        m_indBuf->data.push_back(m_vertexTrack+4);
        m_indBuf->data.push_back(m_vertexTrack+5);
        m_indBuf->data.push_back(m_vertexTrack+1);

        m_indBuf->data.push_back(m_vertexTrack+1);
        m_indBuf->data.push_back(m_vertexTrack+0);
        m_indBuf->data.push_back(m_vertexTrack+4);

        m_indexTrack+=6;
    }

    // Left
    if(CheckBit(sides,EBS_LEFT))
    {
        m_indBuf->data.push_back(m_vertexTrack+4);
        m_indBuf->data.push_back(m_vertexTrack+0);
        m_indBuf->data.push_back(m_vertexTrack+3);

        m_indBuf->data.push_back(m_vertexTrack+3);
        m_indBuf->data.push_back(m_vertexTrack+7);
        m_indBuf->data.push_back(m_vertexTrack+4);

        m_indexTrack+=6;
    }

    // Right
    if(CheckBit(sides,EBS_RIGHT))
    {
        m_indBuf->data.push_back(m_vertexTrack+1);
        m_indBuf->data.push_back(m_vertexTrack+5);
        m_indBuf->data.push_back(m_vertexTrack+6);

        m_indBuf->data.push_back(m_vertexTrack+6);
        m_indBuf->data.push_back(m_vertexTrack+2);
        m_indBuf->data.push_back(m_vertexTrack+1);

        m_indexTrack+=6;
    }
    m_dirty=true;
    m_vertexTrack+=8;
}
