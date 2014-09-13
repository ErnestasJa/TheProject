#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

class Camera;
class Block;
class Chunk;
enum EBlockType;

struct chunkhasher {
    size_t operator()(const glm::vec3& a) const {
          return pow(((a.x+a.y+a.z)+(a.x-a.y-a.z))*((a.x+a.y+a.z)-(a.x-a.y-a.z)),2);
    }
};

inline glm::vec3 WorldToChunkCoords(const glm::vec3 &other)
{
    int cx = glm::floor(other.x / CHUNK_SIZEF);
    int cy = glm::floor(other.y / CHUNK_SIZEF);
    int cz = glm::floor(other.z / CHUNK_SIZEF);

    return glm::vec3(cx,cy,cz);
}

inline glm::vec3 ChunkToWorldCoords(const glm::vec3 &other)
{
    int cx = glm::floor(other.x * CHUNK_SIZEF);
    int cy = glm::floor(other.y * CHUNK_SIZEF);
    int cz = glm::floor(other.z * CHUNK_SIZEF);

    return glm::vec3(cx,cy,cz);
}

inline glm::vec3 ChunkSpaceCoords(const glm::vec3 &pos)
{
	glm::vec3 ats;

	ats.x = (int)pos.x % CHUNK_SIZE;
	if(pos.x<0&&ats.x!=0)
        ats.x= (CHUNK_SIZE)+ats.x;

	ats.y = (int)pos.y % CHUNK_SIZE;
	if(pos.y<0&&ats.y!=0)
        ats.y=(CHUNK_SIZE)+ats.y;

	ats.z = (int)pos.z % CHUNK_SIZE;
	if(pos.z<0&&ats.z!=0)
        ats.z=(CHUNK_SIZE)+ats.z;

	return ats;
}

class ChunkManager
{
public:
    ChunkManager();
    virtual ~ChunkManager();

    void Explode(glm::vec3 pos,float power);

    void SetBlock(glm::vec3 pos,EBlockType type,bool active);
    Block GetBlock(glm::vec3 pos);

    Chunk *GetChunk(glm::vec3 pos);
    Chunk *GetChunkWorld(glm::vec3 pos);
    void SetChunkNeighbours(Chunk* chunk,glm::vec3 pos);

    Chunk *AddChunk(glm::vec3 pos);
    Chunk *AddChunkWorld(glm::vec3 pos);

    void Render(Camera *cam);

    void Clear();
protected:
private:
    std::unordered_map<glm::vec3,Chunk*,chunkhasher> m_chunks;
};

#endif // CHUNKMANAGER_H
