#include "precomp.h"

#include "Chunk.h"

#include "ChunkManager.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"

static float col(int in)
{
    float out=(1.f/255.f)*in;
    return out;
}

static glm::vec4 getTypeCol(uint32_t typ)
{
    glm::vec4 ret;
    int nois=rand()%8;
    switch(typ)
    {
    case EBT_VOIDROCK:
        ret=glm::vec4(col(0+nois),col(0+nois),col(0+nois),1);
        break;
    case EBT_STONE:
        ret=glm::vec4(col(128+nois),col(128+nois),col(128+nois),1);
        break;
    case EBT_DIRT:
        ret=glm::vec4(col(64+nois),col(64+nois),col(0+nois),1);
        break;
    case EBT_GRASS:
        ret=glm::vec4(col(0+nois),col(128+nois),col(0+nois),1);
        break;
    default:
        ret=glm::vec4(col(255-nois),col(255-nois),col(255-nois),1);
        break;
    }
    return ret;
}

Chunk::Chunk(ChunkManager *chunkManager, glm::vec3 chunkPos)
{
    // Create the blocks
    m_chunkManager = chunkManager;
    m_chunkPos = chunkPos;
    m_pBlocks = new Block**[CHUNK_SIZE];
    for(int i = 0; i < CHUNK_SIZE; i++)
    {
        m_pBlocks[i] = new Block*[CHUNK_SIZE];

        for(int j = 0; j < CHUNK_SIZE; j++)
        {
            m_pBlocks[i][j] = new Block[CHUNK_SIZE];
        }
    }
}

Chunk::~Chunk()
{
    // Delete the blocks
    for (int i = 0; i < CHUNK_SIZE; ++i)
    {
        for (int j = 0; j < CHUNK_SIZE; ++j)
        {
            delete [] m_pBlocks[i][j];
        }

        delete [] m_pBlocks[i];
    }
    delete [] m_pBlocks;
}

void Chunk::Rebuild()
{
    Cleanup();

    for (int z = 0; z < CHUNK_SIZE; z++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                if(!m_pBlocks[x][y][z].IsActive()) continue;

                uint32_t flags=0;
                AddBit(flags,EBS_LEFT);
                AddBit(flags,EBS_RIGHT);
                AddBit(flags,EBS_TOP);
                AddBit(flags,EBS_BOTTOM);
                AddBit(flags,EBS_FRONT);
                AddBit(flags,EBS_BACK);

                float xx=m_chunkPos.x*CHUNK_SIZEF+x;
                float yy=m_chunkPos.y*CHUNK_SIZEF+y;
                float zz=m_chunkPos.z*CHUNK_SIZEF+z;

                if(m_chunkManager->GetBlock(glm::vec3(xx-1,yy,zz)).IsActive()) RemoveBit(flags,EBS_LEFT);
                if(m_chunkManager->GetBlock(glm::vec3(xx+1,yy,zz)).IsActive()) RemoveBit(flags,EBS_RIGHT);
                if(m_chunkManager->GetBlock(glm::vec3(xx,yy-1,zz)).IsActive()) RemoveBit(flags,EBS_BOTTOM);
                if(m_chunkManager->GetBlock(glm::vec3(xx,yy+1,zz)).IsActive()) RemoveBit(flags,EBS_TOP);
                if(m_chunkManager->GetBlock(glm::vec3(xx,yy,zz-1)).IsActive()) RemoveBit(flags,EBS_BACK);
                if(m_chunkManager->GetBlock(glm::vec3(xx,yy,zz+1)).IsActive()) RemoveBit(flags,EBS_FRONT);

                if(flags!=0) // Only a visible voxel should be added
                    CreateVoxel(x,y,z,flags,getTypeCol(m_pBlocks[x][y][z].GetBlockType()));
            }
        }
    }

    UpdateMesh();

    m_dirty=false;
}

void Chunk::Set(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active)
{
    if(m_pBlocks[x][y][z].IsActive()==active||(m_pBlocks[x][y][z].IsActive()==active&&m_pBlocks[x][y][z].GetBlockType()==type))
        return;
    m_pBlocks[x][y][z].SetActive(active);
    m_pBlocks[x][y][z].SetBlockType(type);
    m_dirty=true;
}

Block Chunk::Get(uint32_t x,uint32_t y,uint32_t z)
{
    Block b=Block();
    b.SetActive(false);
    b.SetBlockType(EBT_DEFAULT);
    if((x>CHUNK_SIZE-1||x<0)||(y>CHUNK_SIZE-1||y<0)||(z>CHUNK_SIZE-1||z<0))
        return b;
    return m_pBlocks[x][y][z];
}

void Chunk::SetupSphere()
{
    for (int z = 0; z < CHUNK_SIZE; z++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
//                if (sqrt((float) (x-CHUNK_SIZE/2)*(x-CHUNK_SIZE/2) + (y-CHUNK_SIZE/2)*(y-CHUNK_SIZE/2) + (z-CHUNK_SIZE/2)*(z-CHUNK_SIZE/2)) <= CHUNK_SIZE/2)
//                {
                m_pBlocks[x][y][z].SetActive(true);

                m_pBlocks[x][y][z].SetBlockType(EBT_GRASS);
//                }
//                else
//                {
//                    m_pBlocks[x][y][z].SetBlockType(EBT_DEFAULT);
//                }
            }
        }
    }
}

void Chunk::Generate()
{
    for (int z = 0; z < CHUNK_SIZE; z++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                if(!m_pBlocks[x][y][z].IsActive()) continue;

                uint32_t flags=0;
                AddBit(flags,EBS_LEFT);
                AddBit(flags,EBS_RIGHT);
                AddBit(flags,EBS_TOP);
                AddBit(flags,EBS_BOTTOM);
                AddBit(flags,EBS_FRONT);
                AddBit(flags,EBS_BACK);

                if(x > 0)
                    if(m_pBlocks[x-1][y][z].IsActive())
                        RemoveBit(flags,EBS_LEFT);

                if(x < CHUNK_SIZE - 1)
                    if(m_pBlocks[x+1][y][z].IsActive())
                        RemoveBit(flags,EBS_RIGHT);

                if(y > 0)
                    if(m_pBlocks[x][y-1][z].IsActive())
                        RemoveBit(flags,EBS_BOTTOM);

                if(y < CHUNK_SIZE - 1)
                    if(m_pBlocks[x][y+1][z].IsActive())
                        RemoveBit(flags,EBS_TOP);

                if(z > 0)
                    if(m_pBlocks[x][y][z-1].IsActive())
                        RemoveBit(flags,EBS_BACK);

                if(z < CHUNK_SIZE - 1)
                    if(m_pBlocks[x][y][z+1].IsActive())
                        RemoveBit(flags,EBS_FRONT);

                CreateVoxel(x,y,z,flags,glm::vec4(0.5,0.5,0.5,1));
            }
        }
    }
}

