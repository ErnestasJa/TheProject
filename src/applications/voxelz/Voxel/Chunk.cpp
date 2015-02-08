#include "Precomp.h"

#include "Chunk.h"

#include "ChunkManager.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"
#include <boost/foreach.hpp>

Block Chunk::EMPTY_BLOCK=Block();

static intRGBA getTypeCol(uint32_t typ)
{
    switch(typ)
    {
    case EBT_VOIDROCK:
        return VecRGBAToIntRGBA(u8vec4(0,0,0,255));
        break;
    case EBT_STONE:
        return VecRGBAToIntRGBA(u8vec4(128,128,128,255));
        break;
    case EBT_SAND:
        return VecRGBAToIntRGBA(u8vec4(192,192,64,255));
        break;
    case EBT_DIRT:
        return VecRGBAToIntRGBA(u8vec4(64,64,0,255));
        break;
    case EBT_GRASS:
        return VecRGBAToIntRGBA(u8vec4(0,128,0,255));
        break;
    case EBT_LEAF:
        return VecRGBAToIntRGBA(u8vec4(0,192,0,255));
        break;
    case EBT_WOOD:
        return VecRGBAToIntRGBA(u8vec4(128,128,0,255));
        break;
    case EBT_WATER:
        return VecRGBAToIntRGBA(u8vec4(0,0,128,128));
        break;
    default:
        return VecRGBAToIntRGBA(u8vec4(255,255,255,255));
        break;
    }
}

Chunk::Chunk(ChunkManager *chunkManager,const glm::ivec3 &chunkPos, const uint32_t & offset):VoxelMesh(CHUNK_SIZE,false)//,m_pBlocks(boost::extents[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE])
{
    m_pBlocks.resize(CHUNK_SIZE*CHUNK_SIZE*CHUNK_SIZE);
    // Create the blocks
    m_chunkManager = chunkManager;
    m_chunkPos = chunkPos;

    generated=false;
    built=false;
    uploaded=false;

    leftN=rightN=botN=topN=backN=frontN=nullptr;
    _offset=offset;
}

Chunk::~Chunk()
{
    m_pBlocks.clear();
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
                if(!m_pBlocks[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE].active)
                {
                    RemoveVox(x,y,z);
                    continue;
                }
                CreateVox(x,y,z,getTypeCol(m_pBlocks[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE].type));
            }
        }
    }

    //UpdateMesh();

    m_dirty=false;
}

void Chunk::Set(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active)
{
    if(x<0||x>=CHUNK_SIZE||y<0||y>=CHUNK_SIZE||z<0||z>=CHUNK_SIZE) return;
    if(active)
    {
        Block a;
        a.type=type;
        a.active=active;
        m_pBlocks[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE]=a;

        CreateVox(x,y,z,getTypeCol(type));
        m_dirty=true;
    }
    else
    {
        m_pBlocks[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE]=EMPTY_BLOCK;
        RemoveVox(x,y,z);
        m_dirty=true;
    }
}

const Block &Chunk::Get(uint32_t x,uint32_t y,uint32_t z)
{
    if((x>CHUNK_SIZE-1||x<0)||(y>CHUNK_SIZE-1||y<0)||(z>CHUNK_SIZE-1||z<0))
        return EMPTY_BLOCK;
    return m_pBlocks[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE];
}

void Chunk::Fill()
{
    for (int z = 0; z < CHUNK_SIZE; z++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                m_pBlocks[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE].type=EBT_STONE;
                m_pBlocks[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE].active=true;
                CreateVox(x,y,z,getTypeCol(m_pBlocks[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE].type));
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
                if(m_pBlocks[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE].active) ret++;
            }
        }
    }

    return ret;
}

const Voxel& Chunk::GetVoxel(int32_t x,int32_t y, int32_t z)
{
    if(x<0&&leftN!=nullptr)
    {
        return leftN->GetVoxel(CHUNK_SIZE-1,y,z);
    }
    else if(x>CHUNK_SIZE-1&&rightN!=nullptr)
    {
        return rightN->GetVoxel(0,y,z);
    }
    else if(y<0&&botN!=nullptr)
    {
        return botN->GetVoxel(x,CHUNK_SIZE-1,z);
    }
    else if(y>CHUNK_SIZE-1&&topN!=nullptr)
    {
        return topN->GetVoxel(x,0,z);
    }
    else if(z<0&&backN!=nullptr)
    {
        return backN->GetVoxel(x,y,CHUNK_SIZE-1);
    }
    else if(z>CHUNK_SIZE-1&&frontN!=nullptr)
    {
        return frontN->GetVoxel(x,y,0);
    }
    else if(x<0||y<0||z<0||x>CHUNK_SIZE-1||y>CHUNK_SIZE-1||z>CHUNK_SIZE-1)
    {
        return VoxelMesh::EMPTY_VOXEL;
    }
    else
    {
        return m_vox[x+y*CHUNK_SIZE+z*CHUNK_SIZE*CHUNK_SIZE];
    }
}

void Chunk::AddQuadToMesh(const glm::ivec3 * face, const intRGBA &col)
{
    BufferObject<glm::ivec3> *vbo = (BufferObject<glm::ivec3> *) buffers[Mesh::POSITION];
    IndexBufferObject<uint32_t> * ibo = (IndexBufferObject<uint32_t> *) buffers[Mesh::INDICES];
    BufferObject<u8vec4> *cbo = (BufferObject<u8vec4> *) buffers[Mesh::COLOR];

    uint32_t indexStart=_offset+vbo->data.size();

    vbo->data.push_back(m_chunkPos+face[0]);
    vbo->data.push_back(m_chunkPos+face[1]);
    vbo->data.push_back(m_chunkPos+face[2]);
    vbo->data.push_back(m_chunkPos+face[3]);

    u8vec4 _col=IntRGBAToVecRGBA(col);

    cbo->data.push_back(_col);
    cbo->data.push_back(_col);
    cbo->data.push_back(_col);
    cbo->data.push_back(_col);

    ibo->data.push_back(indexStart);
    ibo->data.push_back(indexStart+2);
    ibo->data.push_back(indexStart+3);

    ibo->data.push_back(indexStart);
    ibo->data.push_back(indexStart+1);
    ibo->data.push_back(indexStart+2);
}

