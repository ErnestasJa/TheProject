#include "precomp.h"

#include "Chunk.h"

#include "ChunkManager.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"
#include <boost/foreach.hpp>

static u8vec4 getTypeCol(uint32_t typ)
{
    u8vec4 ret;
    int nois=rand()%8;
    switch(typ)
    {
    case EBT_VOIDROCK:
        ret=u8vec4(0+nois,0+nois,0+nois,255);
        break;
    case EBT_STONE:
        ret=u8vec4(128+nois,128+nois,128+nois,255);
        break;
    case EBT_SAND:
        ret=u8vec4(192+nois,192+nois,64+nois,255);
        break;
    case EBT_DIRT:
        ret=u8vec4(64+nois,64+nois,0+nois,255);
        break;
    case EBT_GRASS:
        ret=u8vec4(0+nois,128+nois,0+nois,255);
        break;
    case EBT_LEAF:
        ret=u8vec4(0+nois,192+nois,0+nois,255);
        break;
    case EBT_WOOD:
        ret=u8vec4(128+nois,128+nois,0+nois,255);
        break;
    case EBT_WATER:
        ret=u8vec4(0+nois,0+nois,128+nois,128);
        break;
    default:
        ret=u8vec4(255-nois,255-nois,255-nois,255);
        break;
    }
    return ret;
}

Chunk::Chunk(ChunkManager *chunkManager, glm::vec3 chunkPos):m_pBlocks(boost::extents[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE])
{
    // Create the blocks
    m_chunkManager = chunkManager;
    m_chunkPos = chunkPos;

    leftN=rightN=botN=topN=backN=frontN=nullptr;
}

Chunk::~Chunk()
{
    Cleanup();
}

void Chunk::UpdateNeighbours()
{
    if(leftN) leftN->Rebuild();
    if(rightN) rightN->Rebuild();
    if(botN) botN->Rebuild();
    if(topN) topN->Rebuild();
    if(backN) backN->Rebuild();
    if(frontN) frontN->Rebuild();
}

void Chunk::Rebuild()
{
    Cleanup();

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                if(!m_pBlocks[x][y][z].IsActive()) continue;

                uint32_t flags=0;
                AddBit(flags,EBS_LEFT);
                AddBit(flags,EBS_RIGHT);
                AddBit(flags,EBS_TOP);
                AddBit(flags,EBS_BOTTOM);
                AddBit(flags,EBS_FRONT);
                AddBit(flags,EBS_BACK);

                if(x>0&&m_pBlocks[x-1][y][z].IsActive())
                {
                    RemoveBit(flags,EBS_LEFT);
                }
                else if(x==0&&leftN!=nullptr&&leftN->Get(15,y,z).IsActive())
                {
                    RemoveBit(flags,EBS_LEFT);
                }

                if(x<CHUNK_SIZE-1&&m_pBlocks[x+1][y][z].IsActive())
                {
                    RemoveBit(flags,EBS_RIGHT);
                }
                else if(x==(CHUNK_SIZE-1)&&rightN!=nullptr&&rightN->Get(0,y,z).IsActive())
                {
                    RemoveBit(flags,EBS_RIGHT);
                }

                if(y>0&&m_pBlocks[x][y-1][z].IsActive())
                {
                    RemoveBit(flags,EBS_BOTTOM);
                }
                else if(y==0&&botN!=nullptr&&botN->Get(x,15,z).IsActive())
                {
                    RemoveBit(flags,EBS_BOTTOM);
                }

                if(y<CHUNK_SIZE-1&&m_pBlocks[x][y+1][z].IsActive())
                {
                    RemoveBit(flags,EBS_TOP);
                }
                else if(y==CHUNK_SIZE-1&&topN!=nullptr&&topN->Get(x,0,z).IsActive())
                {
                    RemoveBit(flags,EBS_TOP);
                }

                if(z>0&&m_pBlocks[x][y][z-1].IsActive())
                {
                    RemoveBit(flags,EBS_BACK);
                }
                else if(z==0&&backN!=nullptr&&backN->Get(x,y,15).IsActive())
                {
                    RemoveBit(flags,EBS_BACK);
                }

                if(z<CHUNK_SIZE-1&&m_pBlocks[x][y][z+1].IsActive())
                {
                    RemoveBit(flags,EBS_FRONT);
                }
                else if(z==(CHUNK_SIZE-1)&&frontN!=nullptr&&frontN->Get(x,y,0).IsActive())
                {
                    RemoveBit(flags,EBS_FRONT);
                }

                if(flags!=0) // Only a visible voxel should be added
                {
                    CreateVoxel(x,y,z,flags,getTypeCol(m_pBlocks[x][y][z].GetBlockType()));
                }
            }
        }
    }

    UpdateMesh();

    m_dirty=false;
}

void Chunk::Set(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active)
{
    m_pBlocks[x][y][z].SetActive(active);
    m_pBlocks[x][y][z].SetBlockType(type);
    m_dirty=true;
}

const Block &Chunk::Get(uint32_t x,uint32_t y,uint32_t z)
{
    if((x>CHUNK_SIZE-1||x<0)||(y>CHUNK_SIZE-1||y<0)||(z>CHUNK_SIZE-1||z<0))
        return EMPTY_BLOCK;
    return m_pBlocks[x][y][z];
}

void Chunk::Fill()
{
    for (int z = 0; z < CHUNK_SIZE; z++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                m_pBlocks[x][y][z].SetBlockType(EBT_STONE);
                m_pBlocks[x][y][z].SetActive(true);
            }
        }
    }
    m_dirty=true;
}

uint32_t Chunk::GetBlockCount()
{
    uint32_t ret=0;

    for (int x = 0; x < CHUNK_SIZE; x++)
    {
        for (int z = 0; z < CHUNK_SIZE; z++)
        {
            for (int y = 0; y < CHUNK_SIZE; y++)
            {
                if(m_pBlocks[x][y][z].IsActive()) ret++;
            }
        }
    }

    return ret;
}

