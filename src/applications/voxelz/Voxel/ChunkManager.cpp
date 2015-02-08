#include "Precomp.h"
#include "Voxel/Chunk.h"
#include "ChunkManager.h"
#include "scenegraph/Camera.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "utility/SimplexNoise.h"
#include "utility/Timer.h"
#include <boost/foreach.hpp>

#define WORLD_HEIGHT 256

ChunkManager::ChunkManager()
{
}

ChunkManager::~ChunkManager()
{
//    int cunt=0;
//    for(auto a:_superChunks)
//    {
//        cunt++;
//        delete a.second;
//    }
//    printf("Cunt is %d\n",cunt);
//    _superChunks.clear();
}

static const char* v3str(const glm::vec3 & vec)
{
    char buf[256];
    sprintf(buf,"vec3(%f %f %f)",vec.x,vec.y,vec.z);
    return std::string(buf).c_str();
}

void ChunkManager::SetBlock(const glm::ivec3 &pos,EBlockType type,bool active)
{
    glm::ivec3 chunkCoords=WorldToSuperChunkCoords(pos),voxelCoords=SuperChunkSpaceCoords(pos);

    if(_superChunks.count(chunkCoords)!=0)
    {
        _superChunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
    else
    {
        AddChunk(chunkCoords);
        _superChunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
}

//! pos: in CHUNK coordinates
SuperChunkPtr ChunkManager::AddChunk(const glm::ivec3 &pos)
{
    if(_superChunks.count(pos)!=0)
    {
        return _superChunks[pos];
    }
    else
    {
        _superChunks[pos]=share(new SuperChunk(this,SuperChunkToWorldCoords(pos)));
        return _superChunks[pos];
    }
}

//! pos: in WORLD coordinates
SuperChunkPtr ChunkManager::AddChunkWorld(const glm::ivec3 &pos)
{
//    glm::ivec3 chunkCoords=WorldToChunkCoords(pos);
//    if(_superChunks.count(chunkCoords)!=0)
//    {
//        return _superChunks[chunkCoords];
//    }
//    else
//    {
//        _superChunks[chunkCoords]=new Chunk(this,ChunkToWorldCoords(chunkCoords));
//        return _superChunks[chunkCoords];
//    }
}

//! pos: in CHUNK coordinates
SuperChunkPtr ChunkManager::GetChunk(const glm::ivec3 &pos)
{
    if(_superChunks.count(pos)!=0)
        return _superChunks[pos];
    else
        return NullSuperChunk;
}

//! pos: in WORLD coordinates
SuperChunkPtr ChunkManager::GetChunkWorld(const glm::ivec3 &pos)
{
    glm::ivec3 chunkCoords=WorldToSuperChunkCoords(pos);
    if(_superChunks.count(chunkCoords)!=0)
        return _superChunks[chunkCoords];
    else
        return NullSuperChunk;
}

const Block &ChunkManager::GetBlock(const glm::ivec3 &pos)
{
    glm::ivec3 chunkCoords=WorldToSuperChunkCoords(pos),voxelCoords=SuperChunkSpaceCoords(pos);
    if(_superChunks.count(chunkCoords)!=0)
        return _superChunks[chunkCoords]->Get(voxelCoords.x,voxelCoords.y,voxelCoords.z);
    else
        return Chunk::EMPTY_BLOCK;
}

void ChunkManager::Render(Camera *cam,ShaderPtr vsh,bool wireframe)
{
    glm::mat4 Model;
    if(wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    }
    BOOST_FOREACH(SuperChunkMap::value_type a,_superChunks)
    {
        a.second->Update();
        //if(!a.second->Empty())
        {
            glm::vec3 pos=glm::vec3(a.first)*SUPERCHUNK_SIZE_BLOCKSF;

            Model = glm::mat4(1.0f);
            Model = glm::translate(Model,pos);
            if(vsh->getparam("M")!=-1) MVar<glm::mat4>(vsh->getparam("M"), "M", Model).Set();
            glm::mat4 MVP=cam->GetProjectionMat()*cam->GetViewMat()*Model;
            glm::mat3 normMatrix = glm::inverse(glm::mat3(cam->GetViewMat()*Model));
            if(vsh->getparam("normMatrix")!=-1) MVar<glm::mat3>(vsh->getparam("normMatrix"), "normMatrix", normMatrix).Set();
            if(vsh->getparam("MVP")!=-1) MVar<glm::mat4>(vsh->getparam("MVP"), "MVP", MVP).Set();
            a.second->Render();
        }
    }
    if(wireframe)
    {
        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    }
}

uint32_t ChunkManager::GetChunkCount()
{
    return _superChunks.size();
}

//uint32_t ChunkManager::GetTotalBlocks()
//{
//    uint32_t ret=0;
//    BOOST_FOREACH(ChunkMap::value_type a,_superChunks)
//    {
//        ret+=a.second->GetBlockCount();
//    }
//    return ret;
//}

//uint32_t ChunkManager::GetTotalFaces()
//{
//    uint32_t totalfaces=0;
//    BOOST_FOREACH(ChunkMap::value_type a,_superChunks)
//    {
//        totalfaces+=a.second->GetFaceCount();
//    }
//    return totalfaces;
//}

