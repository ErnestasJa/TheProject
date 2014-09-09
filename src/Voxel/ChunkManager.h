#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

class Camera;
class Block;
class Chunk;
enum EBlockType;

struct glmvec3comparino
{
    bool operator()(const glm::vec3& a, const glm::vec3& b) const
    {
        return  (a.x<b.x && !helpers::equals(a.x, b.x)) ||
                (helpers::equals(a.x, b.x) && a.y<b.y && !helpers::equals(a.y, b.y)) ||
                (helpers::equals(a.x, b.x) && helpers::equals(a.y, b.y) && a.z<b.z && !helpers::equals(a.z, b.z));
    }
};

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

    void Render(Camera *cam);
protected:
private:
    std::unordered_map<glm::vec3,Chunk*,chunkhasher> m_chunks;
};

#endif // CHUNKMANAGER_H
