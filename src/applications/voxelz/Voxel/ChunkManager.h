#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include "opengl/Shader.h"
#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>

class Camera;
class Block;
class Chunk;
enum EBlockType;

struct chunk_hash : std::unary_function<glm::ivec3, std::size_t>
{
    std::size_t operator()(glm::ivec3 const& v) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, v.x);
        std::size_t a=seed;
        boost::hash_combine(seed, v.y);
        std::size_t b=seed;
        boost::hash_combine(seed, v.z);
        std::size_t c=seed;

        return seed;
    }
};

struct chunk_equal
{
    bool operator() (glm::ivec3 const& a, glm::ivec3 const& b) const
    {
        return a.x==b.x && a.y==b.y && a.z==b.z;
    }
};

struct GLMVec3Compare
{
   bool operator() (const glm::vec3 &a,const glm::vec3 &b) const
   {
       return
        (a.x<b.x || !(a.x==b.x)) ||
        (a.x==b.x && a.y<b.y && !(a.y==b.y)) ||
        (a.x==b.x && a.y==b.y && a.z<b.z && !(a.z==b.z));
   }
};

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

typedef boost::unordered_map<glm::ivec3, ChunkPtr, chunk_hash, chunk_equal> ChunkMap;

class ChunkManager
{
public:
    ChunkManager();
    virtual ~ChunkManager();

    void Generate();

    void NoiseChunk(ChunkPtr chunkToNoise);

    void Explode(const glm::ivec3 &pos,float power);

    void SetBlock(const glm::ivec3 &pos,EBlockType type,bool active);
    const Block &GetBlock(const glm::ivec3 &pos);

    ChunkPtr GetChunk(const glm::ivec3 &pos);
    ChunkPtr GetChunkWorld(const glm::ivec3 &pos);
    void SetChunkNeighbours(ChunkPtr chunk,const glm::ivec3 &pos);

    ChunkPtr AddChunk(const glm::ivec3 &pos);
    ChunkPtr AddChunkWorld(const glm::ivec3 &pos);

    void RemoveChunk(const glm::ivec3 &pos);

    void Render(Camera *cam,ShaderPtr vsh,bool wireframe=false);

    void Clear();

    uint32_t GetChunkCount();

    uint32_t GetTotalBlocks();

    uint32_t GetTotalFaces();
protected:
private:
    ChunkMap m_chunks;
    ChunkPtr NullChunk;
};

#endif // CHUNKMANAGER_H
