#include "Precomp.h"
#include "Voxel/Block.h"
#include "Voxel/Chunk.h"
#include "ChunkManager.h"
#include "Scenegraph/Camera.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "utility/SimplexNoise.h"
#include "utility/timer.h"
#include <boost/foreach.hpp>

ChunkManager::ChunkManager()
{
    int testsize=512;
    int testheight=32;

    for(int x=-testsize; x<testsize; x++)
    {
        for(int z=-testsize; z<testsize; z++)
        {
            float h=scaled_raw_noise_2d(5,testheight,x/256.f,z/256.f);
            for(int y=0; y<testheight; y++)
            {
                if(y==0)
                {
                    SetBlock(glm::vec3(x,y,z),EBT_VOIDROCK,true);
                    continue;
                }
                else if(y==(int)h)
                {
                    float b=scaled_raw_noise_3d(0,testheight,x/256.f,y/256.f,z/256.f);
                    if(b*(rand()%5+1)<32)
                    {
                        SetBlock(glm::vec3(x,y,z),EBT_SAND,true);
                        continue;
                    }
                    SetBlock(glm::vec3(x,y,z),EBT_GRASS,true);
                    continue;
                }
                else if(y>h)
                {
                    if(GetBlock(glm::vec3(x, y - 1, z)).GetBlockType() == EBT_GRASS && (rand() & 0xff) == 0)
                    {
                        // Trunk
                        h = (rand() & 0x3) + 3;
                        for(int i = 0; i < h; i++)
                            SetBlock(glm::vec3(x, y + i, z), EBT_WOOD, true);

                        // Leaves
                        for(int ix = -3; ix <= 3; ix++)
                        {
                            for(int iy = -1; iy <= 1; iy++)
                            {
                                for(int iz = -3; iz <= 3; iz++)
                                {
                                    if(ix * ix + iy * iy + iz * iz < 8 + (rand() & 1) && !GetBlock(glm::vec3(x + ix, y + h + iy, z + iz)).IsActive())
                                        SetBlock(glm::vec3(x + ix, y + h + iy, z + iz), EBT_LEAF, true);
                                }
                            }
                        }
                    }
                    break;
                }
                else
                {
                    SetBlock(glm::vec3(x,y,z),EBT_STONE,true);
                }
            }
        }
    }

    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
    {
        SetChunkNeighbours(a.second,a.first);
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
                    SetBlock(glm::vec3(x,y,z),EBT_AIR,false);
                }
            }
        }
    }
}

void ChunkManager::SetBlock(glm::vec3 pos,EBlockType type,bool active)
{
    glm::vec3 chunkCoords=WorldToChunkCoords(pos),voxelCoords=ChunkSpaceCoords(pos);

    if(m_chunks.count(chunkCoords)!=0)
    {
        m_chunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
    else
    {
        AddChunk(chunkCoords);
        m_chunks[chunkCoords]->Set(voxelCoords.x,voxelCoords.y,voxelCoords.z,type,active);
    }
}

void ChunkManager::SetChunkNeighbours(Chunk* chunk,glm::vec3 pos)
{
    glm::vec3 posXNeg(pos.x-1,pos.y,pos.z),posXPos(pos.x+1,pos.y,pos.z),posYNeg(pos.x,pos.y-1,pos.z),posYPos(pos.x,pos.y+1,pos.z),posZNeg(pos.x,pos.y,pos.z-1),posZPos(pos.x,pos.y,pos.z+1);

    if(m_chunks.count(posXNeg)!=0) m_chunks[pos]->leftN=m_chunks[posXNeg];
    if(m_chunks.count(posXPos)!=0) m_chunks[pos]->rightN=m_chunks[posXPos];

    if(m_chunks.count(posYNeg)!=0) m_chunks[pos]->botN=m_chunks[posYNeg];
    if(m_chunks.count(posYPos)!=0) m_chunks[pos]->topN=m_chunks[posYPos];

    if(m_chunks.count(posZNeg)!=0) m_chunks[pos]->backN=m_chunks[posZNeg];
    if(m_chunks.count(posZPos)!=0) m_chunks[pos]->frontN=m_chunks[posZPos];
}

//! pos: in CHUNK coordinates
Chunk *ChunkManager::AddChunk(glm::vec3 pos)
{
    if(m_chunks.count(pos)!=0)
    {
        return m_chunks[pos];
    }
    else
    {
        m_chunks[pos]=new Chunk(this,ChunkToWorldCoords(pos));
        SetChunkNeighbours(m_chunks[pos],pos);
        return m_chunks[pos];
    }
}

//! pos: in WORLD coordinates
Chunk *ChunkManager::AddChunkWorld(glm::vec3 pos)
{
    glm::vec3 chunkCoords=WorldToChunkCoords(pos);
    if(m_chunks.count(chunkCoords)!=0)
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
    if(m_chunks.count(pos)!=0)
        return m_chunks[pos];
    else
        return nullptr;
}

//! pos: in WORLD coordinates
Chunk *ChunkManager::GetChunkWorld(glm::vec3 pos)
{
    glm::vec3 chunkCoords=WorldToChunkCoords(pos);
    if(m_chunks.count(chunkCoords)!=0)
        return m_chunks[chunkCoords];
    else
        return nullptr;
}

Block ChunkManager::GetBlock(glm::vec3 pos)
{
    glm::vec3 chunkCoords=WorldToChunkCoords(pos),voxelCoords=ChunkSpaceCoords(pos);
    if(m_chunks.count(chunkCoords)!=0)
        return m_chunks[chunkCoords]->Get(voxelCoords.x,voxelCoords.y,voxelCoords.z);
    else
        return EMPTY_BLOCK;
}

void ChunkManager::Render(Camera *cam,ShaderPtr vsh)
{
    glm::mat4 Model;

    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
    {
        glm::vec3 pos=a.first*CHUNK_SIZEF;

        Model = glm::mat4(1.0f);
        Model = glm::translate(Model,pos);
        MVar<glm::mat4>(vsh->getparam("M"), "M", Model).Set();
        glm::mat4 MVP=cam->GetProjectionMat()*cam->GetViewMat()*Model;
        glm::mat3 normMatrix = glm::transpose(glm::inverse(glm::mat3(cam->GetViewMat()*Model)));
        MVar<glm::mat3>(vsh->getparam("normMatrix"), "normMatrix", normMatrix).Set();
        a.second->Render();
    }
}

void ChunkManager::Clear()
{
//    for(boost::unordered_map<glm::vec3,Chunk*,chunkhasher>::iterator it=m_chunks.begin(); it!=m_chunks.end();)
//    {
//        m_chunks.erase(it++);
//    }
    m_chunks.clear();
}

uint32_t ChunkManager::GetChunkCount()
{
    return m_chunks.size();
}

uint32_t ChunkManager::GetTotalBlocks()
{
    uint32_t ret=0;
    BOOST_FOREACH(ChunkMap::value_type a,m_chunks)
    {
        ret+=a.second->GetBlockCount();
    }
    return ret;
}

