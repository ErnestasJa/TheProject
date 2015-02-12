#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"

#define CHUNK_SIZE 16
#define CHUNK_SIZEF 16.f
#define CHUNK_BLOCK_SIZE ((CHUNK_SIZE)*(CHUNK_SIZE)*(CHUNK_SIZE))
#define CHUNK_MESH_SIZE ((CHUNK_SIZE)*(CHUNK_SIZE)*(CHUNK_SIZE))

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

class Chunk
{
public:
    uint32_t id;
    uint32_t offset;
    bool empty,generated,built,uploaded;
    glm::ivec3 position;

    static Block EMPTY_BLOCK;

    VoxMeshData<u16vec3,u8vec4,uint32_t> meshData;

    typedef std::shared_ptr<Chunk> _ChunkPtr;
    _ChunkPtr leftN,rightN,botN,topN,backN,frontN;
private:
    ChunkManager *_chunkManager;
    vector<Block> _blocks;
public:
    BOOST_MOVABLE_BUT_NOT_COPYABLE(Chunk)

    Chunk(ChunkManager *chunkManager,const glm::ivec3 &position, const uint32_t & offset);
    virtual ~Chunk();

    void Fill();
    void FillCheckerboard();
    void UpdateNeighbours(uint32_t x,uint32_t y,uint32_t z);

    void SetBlock(uint32_t x,uint32_t y,uint32_t z,EBlockType type,bool active);
    const Block &GetBlock(uint32_t x,uint32_t y,uint32_t z);

    uint32_t GetBlockCount();

    const Block & ElementAt(int32_t x,int32_t y, int32_t z);

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
};
typedef Chunk::_ChunkPtr ChunkPtr;
typedef boost::unordered_map<glm::ivec3, ChunkPtr, ivec3hash, ivec3equal> ChunkMap;

#endif // CHUNK_H
