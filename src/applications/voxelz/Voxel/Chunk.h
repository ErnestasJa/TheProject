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
    static Block EMPTY_BLOCK;
    BOOST_MOVABLE_BUT_NOT_COPYABLE(Chunk)

    Chunk(ChunkManager *chunkManager, glm::ivec3 chunkPos);
    virtual ~Chunk();

    void Fill();
    void Rebuild();
    void UpdateNeighbours();

    void Set(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active);
    const Block &Get(uint32_t x,uint32_t y,uint32_t z);
    const glm::ivec3 &GetPosition()
    {
        return m_chunkPos;
    }

    uint32_t GetBlockCount();
private:
    ChunkManager *m_chunkManager;
    glm::ivec3 m_chunkPos;
    vector<Block> m_pBlocks;
    const Voxel& GetVoxel(int32_t x,int32_t y, int32_t z);
public:
    typedef Chunk* _ChunkPtr;
    _ChunkPtr leftN,rightN,botN,topN,backN,frontN;
};
typedef Chunk::_ChunkPtr ChunkPtr;
#endif // CHUNK_H
