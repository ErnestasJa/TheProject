#ifndef CHUNK_H
#define CHUNK_H

#include "VoxelMesh.h"
#include "Block.h"

#define CHUNK_SIZE 16
#define CHUNK_SIZEF 16.f

class Chunk:public VoxelMesh
{
public:
    Chunk();
    ~Chunk();

    void SetupSphere();
    void Generate();

    void Rebuild();

    void Set(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active);
    Block Get(uint32_t x,uint32_t y,uint32_t z);
private:
    // The blocks data
    Block*** m_pBlocks;
};
#endif // CHUNK_H
