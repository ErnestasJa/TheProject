#ifndef CHUNK_H
#define CHUNK_H

#include "VoxelMesh.h"
#include "Block.h"

#define CHUNK_SIZE 16
#define CHUNK_SIZEF 16.f
class ChunkManager;

inline glm::ivec3 WorldToChunkCoords(const glm::ivec3 &other)
{
    int cx = glm::floor(other.x / CHUNK_SIZEF);
    int cy = glm::floor(other.y / CHUNK_SIZEF);
    int cz = glm::floor(other.z / CHUNK_SIZEF);

    return glm::ivec3(cx,cy,cz);
}

inline glm::ivec3 ChunkToWorldCoords(const glm::ivec3 &other)
{
    int cx = other.x * CHUNK_SIZEF;
    int cy = other.y * CHUNK_SIZEF;
    int cz = other.z * CHUNK_SIZEF;

    return glm::ivec3(cx,cy,cz);
}

inline glm::ivec3 ChunkSpaceCoords(const glm::ivec3 &pos)
{
    glm::ivec3 ats;

    ats.x = pos.x % CHUNK_SIZE;
    if(pos.x<0&&ats.x!=0)
        ats.x= (CHUNK_SIZE)+ats.x;

    ats.y = pos.y % CHUNK_SIZE;
    if(pos.y<0&&ats.y!=0)
        ats.y=(CHUNK_SIZE)+ats.y;

    ats.z = pos.z % CHUNK_SIZE;
    if(pos.z<0&&ats.z!=0)
        ats.z=(CHUNK_SIZE)+ats.z;

    return ats;
}

class Chunk:public VoxelMesh
{
public:
    static Block EMPTY_BLOCK;
    BOOST_MOVABLE_BUT_NOT_COPYABLE(Chunk)

    Chunk(ChunkManager *chunkManager,const glm::ivec3 &chunkPos, const uint32_t & offset);
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
    void AddQuadToMesh(const u8vec3 * face, const intRGBA &col);

private:
    ChunkManager *m_chunkManager;
    glm::ivec3 m_chunkPos;
    vector<Block> m_pBlocks;
    const Voxel& GetVoxel(int32_t x,int32_t y, int32_t z);
public:
    uint32_t _id;
    typedef std::shared_ptr<Chunk> _ChunkPtr;
    _ChunkPtr leftN,rightN,botN,topN,backN,frontN;
    uint32_t _offset;
    bool generated,built,uploaded;
};
typedef Chunk::_ChunkPtr ChunkPtr;
typedef boost::unordered_map<glm::ivec3, ChunkPtr, ivec3hash, ivec3equal> ChunkMap;
#endif // CHUNK_H
