#include "precomp.h"

#include "Chunk.h"

#include "opengl/Mesh.h"
#include "resources/ResourceCache.h"

Chunk::Chunk()
{
    // Create the blocks
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

void Chunk::Update(float dt)
{
    printf("Updating...\n");
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

void Chunk::Set(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active)
{
    m_pBlocks[x][y][z].SetActive(active);
    m_pBlocks[x][y][z].SetBlockType(type);
    m_dirty=true;
}

void Chunk::SetupSphere()
{
    for (int z = 0; z < CHUNK_SIZE; z++)
    {
        for (int y = 0; y < CHUNK_SIZE; y++)
        {
            for (int x = 0; x < CHUNK_SIZE; x++)
            {
                if (sqrt((float) (x-CHUNK_SIZE/2)*(x-CHUNK_SIZE/2) + (y-CHUNK_SIZE/2)*(y-CHUNK_SIZE/2) + (z-CHUNK_SIZE/2)*(z-CHUNK_SIZE/2)) <= CHUNK_SIZE/2)
                {
                    m_pBlocks[x][y][z].SetActive(true);

                    m_pBlocks[x][y][z].SetBlockType(EBT_GRASS);
                }
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
