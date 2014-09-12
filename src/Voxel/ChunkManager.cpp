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
//    int testsize=16;
//    int testheight=32;
//
//    for(int x=-testsize; x<testsize; x++)
//    {
//        for(int z=-testsize; z<testsize; z++)
//        {
//            float h=scaled_raw_noise_2d(0,testheight,x/128.f,z/128.f);
//            for(int y=0; y<testheight; y++)
//            {
//                if(y==0)
//                {
//                    SetBlock(glm::vec3(x,y,z),EBT_VOIDROCK,true);
//                    continue;
//                }
//                else if(y==(int)h)
//                {
//                    SetBlock(glm::vec3(x,y,z),EBT_GRASS,true);
//                    continue;
//                }
//                else if(y>h)
//                {
//                    if(GetBlock(glm::vec3(x, y - 1, z)).GetBlockType() == EBT_GRASS && (rand() & 0xff) == 0)
//                    {
//                        // Trunk
//                        h = (rand() & 0x3) + 3;
//                        for(int i = 0; i < h; i++)
//                            SetBlock(glm::vec3(x, y + i, z), EBT_WOOD, true);
//
//                        // Leaves
//                        for(int ix = -3; ix <= 3; ix++)
//                        {
//                            for(int iy = -1; iy <= 1; iy++)
//                            {
//                                for(int iz = -3; iz <= 3; iz++)
//                                {
//                                    if(ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !GetBlock(glm::vec3(x + ix, y + h + iy, z + iz)).IsActive())
//                                        SetBlock(glm::vec3(x + ix, y + h + iy, z + iz), EBT_LEAF, true);
//                                }
//                            }
//                        }
//                    }
//                    break;
//                }
//                else
//                {
//                    SetBlock(glm::vec3(x,y,z),EBT_STONE,true);
//                }
//            }
//        }
//    }
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

