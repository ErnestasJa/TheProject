#ifndef CHUNK_H
#define CHUNK_H

#include "VoxelMesh.h"
#include "Block.h"

#define CHUNK_SIZE 16
#define CHUNK_SIZEF 16.f
class ChunkManager;

static Block EMPTY_BLOCK=Block();

typedef boost::multi_array<Block, 3> BlockArray;

class Chunk:public VoxelMesh
{
public:
    Chunk(ChunkManager *chunkManager, glm::vec3 chunkPos);
    ~Chunk();

    void Fill();
    void Rebuild();
    void UpdateNeighbours();

    void Set(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active);
    const Block &Get(uint32_t x,uint32_t y,uint32_t z);

    uint32_t GetBlockCount();
private:
    ChunkManager *m_chunkManager;
    glm::vec3 m_chunkPos;
    BlockArray m_pBlocks;
    void GetVoxel(Voxel &vox,int32_t x,int32_t y, int32_t z);
public:
    typedef std::shared_ptr<Chunk> chkptr;
    chkptr leftN,rightN,botN,topN,backN,frontN;
};
typedef Chunk::chkptr ChunkPtr;
#endif // CHUNK_H
