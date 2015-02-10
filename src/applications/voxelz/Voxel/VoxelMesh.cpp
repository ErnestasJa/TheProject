#include "Precomp.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"
#include "Chunk.h"
#include "VoxelMesh.h"
#include "utility/Timer.h"
#include "GreedyMeshBuilder.h"

Voxel VoxelMesh::EMPTY_VOXEL=Voxel();

VoxelMesh::VoxelMesh(uint32_t size,bool init)
{
    _vox.reserve(size*size*size);

    _size=size;
    _dirty=false;

    buffers[Mesh::POSITION] = new BufferObject<glm::ivec3>();
    buffers[Mesh::COLOR] = new BufferObject<u8vec4>();
    buffers[Mesh::INDICES] = new IndexBufferObject<uint32_t>();

//    ((BufferObject<glm::ivec3> *) buffers[Mesh::POSITION])->data.reserve((uint32_t)glm::pow(size/2.f,3.f));
//    ((BufferObject<u8vec4> *) buffers[Mesh::COLOR])->data.reserve((uint32_t)glm::pow(size/2.f,3.f));
//    ((BufferObject<uint32_t> *) buffers[Mesh::INDICES])->data.reserve((uint32_t)glm::pow(size/2.f,3.f)*36);

    _faceCount=0;
    _empty=true;

    if(init)
    Init();
}

VoxelMesh::~VoxelMesh()
{
    Cleanup();
}

bool VoxelMesh::Empty()
{
    return _empty;
}

void VoxelMesh::Render(bool wireframe)
{
    if(wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }

    if(!Empty()&&!_dirty)
    {
        Mesh::Render();
    }

    if(wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
}

void VoxelMesh::Cleanup()
{
    if(!Empty())
    {
        freeVector(_vox);

        freeVector(((BufferObject<glm::ivec3> *) buffers[Mesh::POSITION])->data);
        freeVector(((BufferObject<u8vec4> *) buffers[Mesh::COLOR])->data);
        freeVector(((BufferObject<uint32_t> *) buffers[Mesh::INDICES])->data);

        _empty=true;
    }
}

void VoxelMesh::ClearBuffers()
{
    freeVector(((BufferObject<glm::ivec3> *) buffers[Mesh::POSITION])->data);
    freeVector(((BufferObject<u8vec4> *) buffers[Mesh::COLOR])->data);
}

void VoxelMesh::UpdateMesh()
{
    if(!Empty())
    {
        GreedyMeshBuilder::GreedyBuild(this);

        UploadBuffers();
        ClearBuffers();

        RecalculateAABB<glm::ivec3>();
    }
}

const Voxel & VoxelMesh::GetVoxel(int32_t x,int32_t y, int32_t z)
{
    if(x>_size-1 || x<0 || y>_size-1 || y<0 || z>_size-1 || z<0)
    {
        return VoxelMesh::EMPTY_VOXEL;
    }
    else
    {
        return _vox[x+y*_size+z*_size*_size];
    }
}

void VoxelMesh::CreateVox(int32_t x, int32_t y, int32_t z, const intRGBA &col)
{
    Voxel vox;
    vox.active=true;
    vox.color=col;
    if(_empty)
        _empty=false;
    _vox[x+y*_size+z*_size*_size]=vox;
}

void VoxelMesh::RemoveVox(int32_t x, int32_t y, int32_t z)
{
    _vox[x+y*_size+z*_size*_size]=EMPTY_VOXEL;
}

