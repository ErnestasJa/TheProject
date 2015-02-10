#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H
#define WORLD_HEIGHT 128
#define WORLD_HEIGHTF 128.f

#include "opengl/Shader.h"

#include "SuperChunk.h"

class Camera;
enum EBlockType;

class ChunkManager
{
public:
    ChunkManager();
    virtual ~ChunkManager();

    void SetBlock(const glm::ivec3 &pos,EBlockType type,bool active);
    const Block &GetBlock(const glm::ivec3 &pos);

    SuperChunkPtr AddSuperChunk(const glm::ivec3 &pos);
    SuperChunkPtr GetSuperChunk(const glm::ivec3 &pos);

    uint32_t GetChunkCount();

    void Render(Camera *cam,ShaderPtr vsh,bool wireframe=false);
protected:
private:
    SuperChunkMap _superChunks;
    static SuperChunk NULL_SUPERCHUNK;
};

#endif // CHUNKMANAGER_H
