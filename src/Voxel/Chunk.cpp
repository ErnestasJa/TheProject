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

void Chunk::Render()
{
    m_mesh->Render();
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

int vertTrack,indexTrack;
void Chunk::Generate()
{
    vertTrack=0,indexTrack=0;

    m_posBuf = new BufferObject<glm::vec3>();
    m_colBuf = new BufferObject<glm::vec4>();
    m_indBuf=new IndexBufferObject<uint32_t>();

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

                CreateCube(x,y,z,flags);
                indexTrack+=8;
            }
        }
    }

    m_mesh = share(new Mesh());
    m_mesh->buffers[Mesh::POSITION] = m_posBuf;
    m_mesh->buffers[Mesh::COLOR] = m_colBuf;
    m_mesh->buffers[Mesh::INDICES] = m_indBuf;
    m_mesh->Init();
}

void Chunk::CreateCube(float x, float y, float z, uint32_t sides)
{
    float BLOCK_RENDER_SIZE=0.5f;
    /// - - +
    glm::vec3 p1(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
    /// + - +
    glm::vec3 p2(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
    /// + + +
    glm::vec3 p3(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);
    /// - + +
    glm::vec3 p4(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z+BLOCK_RENDER_SIZE);

    /// - - -
    glm::vec3 p5(x-BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
    /// + - -
    glm::vec3 p6(x+BLOCK_RENDER_SIZE, y-BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
    /// + + -
    glm::vec3 p7(x+BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);
    /// - + -
    glm::vec3 p8(x-BLOCK_RENDER_SIZE, y+BLOCK_RENDER_SIZE, z-BLOCK_RENDER_SIZE);

    m_posBuf->data.push_back(p1);
    m_posBuf->data.push_back(p2);
    m_posBuf->data.push_back(p3);
    m_posBuf->data.push_back(p4);
    m_posBuf->data.push_back(p5);
    m_posBuf->data.push_back(p6);
    m_posBuf->data.push_back(p7);
    m_posBuf->data.push_back(p8);

    glm::vec4 col=glm::vec4(1.f/(std::rand()%64),1.f/(std::rand()%64),1.f/(std::rand()%64),1);
    m_colBuf->data.push_back(col);
    m_colBuf->data.push_back(col);
    m_colBuf->data.push_back(col);
    m_colBuf->data.push_back(col);
    m_colBuf->data.push_back(col);
    m_colBuf->data.push_back(col);
    m_colBuf->data.push_back(col);
    m_colBuf->data.push_back(col);

    // Front
    if(CheckBit(sides,EBS_FRONT))
    {
        m_indBuf->data.push_back(indexTrack+0);
        m_indBuf->data.push_back(indexTrack+1);
        m_indBuf->data.push_back(indexTrack+2);

        m_indBuf->data.push_back(indexTrack+2);
        m_indBuf->data.push_back(indexTrack+3);
        m_indBuf->data.push_back(indexTrack+0);
    }

    // Top
    if(CheckBit(sides,EBS_TOP))
    {
        m_indBuf->data.push_back(indexTrack+3);
        m_indBuf->data.push_back(indexTrack+2);
        m_indBuf->data.push_back(indexTrack+6);

        m_indBuf->data.push_back(indexTrack+6);
        m_indBuf->data.push_back(indexTrack+7);
        m_indBuf->data.push_back(indexTrack+3);
    }

    // Back
    if(CheckBit(sides,EBS_BACK))
    {
        m_indBuf->data.push_back(indexTrack+7);
        m_indBuf->data.push_back(indexTrack+6);
        m_indBuf->data.push_back(indexTrack+5);

        m_indBuf->data.push_back(indexTrack+5);
        m_indBuf->data.push_back(indexTrack+4);
        m_indBuf->data.push_back(indexTrack+7);
    }

    // Bottom
    if(CheckBit(sides,EBS_BOTTOM))
    {
        m_indBuf->data.push_back(indexTrack+4);
        m_indBuf->data.push_back(indexTrack+5);
        m_indBuf->data.push_back(indexTrack+1);

        m_indBuf->data.push_back(indexTrack+1);
        m_indBuf->data.push_back(indexTrack+0);
        m_indBuf->data.push_back(indexTrack+4);
    }

    // Left
    if(CheckBit(sides,EBS_LEFT))
    {
        m_indBuf->data.push_back(indexTrack+4);
        m_indBuf->data.push_back(indexTrack+0);
        m_indBuf->data.push_back(indexTrack+3);

        m_indBuf->data.push_back(indexTrack+3);
        m_indBuf->data.push_back(indexTrack+7);
        m_indBuf->data.push_back(indexTrack+4);
    }

    // Right
    if(CheckBit(sides,EBS_RIGHT))
    {
        m_indBuf->data.push_back(indexTrack+1);
        m_indBuf->data.push_back(indexTrack+5);
        m_indBuf->data.push_back(indexTrack+6);

        m_indBuf->data.push_back(indexTrack+6);
        m_indBuf->data.push_back(indexTrack+2);
        m_indBuf->data.push_back(indexTrack+1);
    }
}
