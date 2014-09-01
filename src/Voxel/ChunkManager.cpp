#include "Precomp.h"
#include "Voxel/Block.h"
#include "Voxel/Chunk.h"
#include "ChunkManager.h"
#include "Scenegraph/Camera.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"

ChunkManager::ChunkManager()
{
    //ctor
    m_chunks[glm::vec3(0,0,0)]=new Chunk();
    m_chunks[glm::vec3(0,0,0)]->SetupSphere();
    m_chunks[glm::vec3(0,0,0)]->Generate();
    m_chunks[glm::vec3(1,1,1)]=new Chunk();
    m_chunks[glm::vec3(1,1,1)]->SetupSphere();
    m_chunks[glm::vec3(1,1,1)]->Generate();
    m_chunks[glm::vec3(-1,-1,-1)]=new Chunk();
    m_chunks[glm::vec3(-1,-1,-1)]->SetupSphere();
    m_chunks[glm::vec3(-1,-1,-1)]->Generate();

//Set(glm::vec3(0,0,1),EBT_GRASS,true);
//Set(glm::vec3(1,0,0),EBT_GRASS,true);
//Set(glm::vec3(1,0,1),EBT_GRASS,true);
//Set(glm::vec3(0,0,0),EBT_GRASS,true);
//
//Set(glm::vec3(0,0,-1),EBT_GRASS,true);
//Set(glm::vec3(-1,0,0),EBT_GRASS,true);
//Set(glm::vec3(-2,0,-2),EBT_GRASS,true);
//Set(glm::vec3(-1,0,-1),EBT_GRASS,true);

//Set(0,0,1,EBT_GRASS,true);
//Set(0,0,0,EBT_GRASS,true);
//Set(0,0,0,EBT_GRASS,true);
//Set(0,0,0,EBT_GRASS,true);
}

ChunkManager::~ChunkManager()
{
    //dtor
}

void ChunkManager::Set(glm::vec3 pos,EBlockType type,bool active)
{
    printf("CHUNK MANAGER SET(RAW): (%.2f %.2f %.2f)\n",pos.x,pos.y,pos.z);
    glm::vec3 chunkCoords=WorldToChunkCoords(pos),voxelCoords=ChunkSpaceCoords(pos);

    printf("CHUNK MANAGER SET(CONVERTED): (%.2f %.2f %.2f)\n",voxelCoords.x,voxelCoords.y,voxelCoords.z);

    if(m_chunks.find(chunkCoords)!=m_chunks.end())
    {
        m_chunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
    else
    {
        m_chunks[chunkCoords]=new Chunk();
        m_chunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
}

Block ChunkManager::Get(glm::vec3 pos)
{
    glm::vec3 chunkCoords=WorldToChunkCoords(pos),voxelCoords=ChunkSpaceCoords(pos);
    if(m_chunks.find(chunkCoords)!=m_chunks.end())
    return m_chunks[chunkCoords]->Get(voxelCoords.x,voxelCoords.y,voxelCoords.z);
    else
        return Block();
}

void ChunkManager::Render(Camera *cam)
{

    glm::mat4 Model,MVP;

    for(std::pair<glm::vec3,Chunk*> a:m_chunks)
    {
        Model = glm::mat4(1.0f);
        Model = glm::translate(Model,a.first*CHUNK_SIZEF);
        MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        a.second->Render();
    }
}
