#ifndef CHUNK_H
#define CHUNK_H

#include "VoxelMesh.h"
#include "Block.h"

#define CHUNK_SIZE 16
#define CHUNK_SIZEF 16.f
class ChunkManager;
class Chunk:public VoxelMesh
{
public:
    Chunk(ChunkManager *chunkManager, glm::vec3 chunkPos);
    ~Chunk();

    void SetupSphere();
    void Generate();

    void Rebuild();

    void Set(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active);
    Block Get(uint32_t x,uint32_t y,uint32_t z);
private:
    ChunkManager *m_chunkManager;
    glm::vec3 m_chunkPos;
    Block*** m_pBlocks;
};
#endif // CHUNK_H
