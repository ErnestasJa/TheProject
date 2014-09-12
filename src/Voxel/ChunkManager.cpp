#include "Precomp.h"
#include "Voxel/Block.h"
#include "Voxel/Chunk.h"
#include "ChunkManager.h"
#include "Scenegraph/Camera.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "utility/SimplexNoise.h"
#include "utility/timer.h"

ChunkManager::ChunkManager()
{
    int testsize=128;
    int testheight=64;

    for(int i=-testsize; i<testsize; i++)
    {
        for(int j=-testsize; j<testsize; j++)
        {
            float h=scaled_raw_noise_2d(0,testheight,i/256.f,j/256.f);
            for(int y=0; y<testheight; y++)
            {
                if(y==0)
                {
                    SetBlock(glm::vec3(i,y,j),EBT_VOIDROCK,true);
                    continue;
                }
                else if(y==(int)h)
                {
                    SetBlock(glm::vec3(i,y,j),EBT_GRASS,true);
                    continue;
                }
                else if(y>h)
                {
                    break;
                }
                else
                {
                    SetBlock(glm::vec3(i,y,j),EBT_STONE,true);
                }
            }
        }
    }
}

ChunkManager::~ChunkManager()
{
    //dtor
}

static const char* v3str(const glm::vec3 & vec)
{
    char buf[256];
    sprintf(buf,"vec3(%f %f %f)",vec.x,vec.y,vec.z);
    return std::string(buf).c_str();
}

void ChunkManager::Explode(glm::vec3 pos,float power)
{
    glm::vec3 startpos=pos-power;
    glm::vec3 endpos=pos+power;

    printf("BOOM! Position:%s Startbound:%s Endbound:%s \n",v3str(pos),v3str(startpos),v3str(endpos));

    for(int x=startpos.x; x<endpos.x; x++)
    {
        for(int y=startpos.y; y<endpos.y; y++)
        {
            for(int z=startpos.z; z<endpos.z; z++)
            {
                if (sqrt((float) glm::pow((pos.x-x),2.f) + glm::pow((pos.y-y),2.f) + glm::pow((pos.z-z),2.f)) <= power/2)
                {
                    SetBlock(glm::vec3(x,y,z),EBT_DEFAULT,false);
                }
            }
        }
    }
}

void ChunkManager::SetBlock(glm::vec3 pos,EBlockType type,bool active)
{
    glm::vec3 chunkCoords=WorldToChunkCoords(pos),voxelCoords=ChunkSpaceCoords(pos);

    if(m_chunks.find(chunkCoords)!=m_chunks.end())
    {
        m_chunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
    else
    {
        m_chunks[chunkCoords]=new Chunk(this,chunkCoords);
        m_chunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
}

//! pos: in CHUNK coordinates
Chunk *ChunkManager::AddChunk(glm::vec3 pos)
{
    if(m_chunks.find(pos)!=m_chunks.end())
    {
        return m_chunks[pos];
    }
    else
    {
        m_chunks[pos]=new Chunk(this,ChunkToWorldCoords(pos));
        return m_chunks[pos];
    }
}

//! pos: in WORLD coordinates
Chunk *ChunkManager::AddChunkWorld(glm::vec3 pos)
{
    glm::vec3 chunkCoords=WorldToChunkCoords(pos);
    if(m_chunks.find(chunkCoords)!=m_chunks.end())
    {
        return m_chunks[chunkCoords];
    }
    else
    {
        m_chunks[chunkCoords]=new Chunk(this,ChunkToWorldCoords(chunkCoords));
        return m_chunks[chunkCoords];
    }
}

//! pos: in CHUNK coordinates
Chunk *ChunkManager::GetChunk(glm::vec3 pos)
{
    if(m_chunks.find(pos)!=m_chunks.end())
        return m_chunks[pos];
    else
        return nullptr;
}

//! pos: in WORLD coordinates
Chunk *ChunkManager::GetChunkWorld(glm::vec3 pos)
{
    glm::vec3 chunkCoords=WorldToChunkCoords(pos);
    if(m_chunks.find(chunkCoords)!=m_chunks.end())
        return m_chunks[chunkCoords];
    else
        return nullptr;
}

Block ChunkManager::GetBlock(glm::vec3 pos)
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
        glm::vec3 pos=a.first*CHUNK_SIZEF;

        Model = glm::mat4(1.0f);
        Model = glm::translate(Model,pos);
        MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        a.second->Render();
    }
}

