#include "Precomp.h"
#include "Voxel/Chunk.h"
#include "ChunkManager.h"
#include "scenegraph/Camera.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "utility/SimplexNoise.h"
#include "utility/Timer.h"
#include <boost/foreach.hpp>

ChunkManager::ChunkManager()
{

//    char * buf;
//    uint32_t len;
//    len=helpers::read("res/test1.bvox",buf);
//    uint32_t * data = (uint32_t*)((void*)&buf[0]);
//
//    uint32_t voxel_count = data[0];
//    data++;
//
//    std::cout << "File len: " << len << std::endl;
//    std::cout << "Voxel count: " << voxel_count << std::endl;
//
//    for(int i = 0; i < voxel_count; i++)
//    {
//        uint32_t x = data[0], y = data[1], z = data[2];
//        SetBlock(glm::ivec3(x,y,z),EBT_CLOUD,true);
//        data+=3;
//    }
//
//    delete[] buf;
//
//    for(auto a:_superChunks)
//    {
//        for(auto b:a.second->_chunks)
//        {
//            glm::ivec3 pos=b.first;
//            b.second->leftN=a.second->GetChunk(glm::ivec3(pos.x-1,pos.y,pos.z));
//            b.second->rightN=a.second->GetChunk(glm::ivec3(pos.x+1,pos.y,pos.z));
//            b.second->botN=a.second->GetChunk(glm::ivec3(pos.x,pos.y-1,pos.z));
//            b.second->topN=a.second->GetChunk(glm::ivec3(pos.x,pos.y+1,pos.z));
//            b.second->frontN=a.second->GetChunk(glm::ivec3(pos.x,pos.y,pos.z+1));
//            b.second->backN=a.second->GetChunk(glm::ivec3(pos.x,pos.y,pos.z-1));
//            b.second->generated=true;
//        }
//
//    }

//    for(int x=0; x<8; x++)
//        for(int z=0; z<8; z++)
//    {
//        AddSuperChunk(glm::ivec3(x,0,z));
//        _superChunks[glm::ivec3(x,0,z)]->Fill();
//    }
}

ChunkManager::~ChunkManager()
{
    _superChunks.clear();
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
        AddSuperChunk(chunkCoords);
        //_superChunks[chunkCoords]->Fill();
        _superChunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
}

//! pos: in CHUNK coordinates
SuperChunkPtr ChunkManager::AddSuperChunk(const glm::ivec3 &pos)
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

void ChunkManager::RemoveSuperChunk(const glm::ivec3 &pos)
{
    if(_superChunks.count(pos)!=0)
    {
        _superChunks.erase(pos);
    }
}

//! pos: in SUPER_CHUNK coordinates
SuperChunkPtr ChunkManager::GetSuperChunk(const glm::ivec3 &pos)
{
    if(_superChunks.count(pos)!=0)
        return _superChunks[pos];
    else
        return nullptr;
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

