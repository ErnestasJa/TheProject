#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

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

    SuperChunkPtr AddChunk(const glm::ivec3 &pos);
    SuperChunkPtr AddChunkWorld(const glm::ivec3 &pos);
    SuperChunkPtr GetChunk(const glm::ivec3 &pos);
    SuperChunkPtr GetChunkWorld(const glm::ivec3 &pos);

    uint32_t GetChunkCount();

    void Render(Camera *cam,ShaderPtr vsh,bool wireframe=false);
protected:
private:
    SuperChunkMap _superChunks;
    SuperChunkPtr NullSuperChunk;
};

#endif // CHUNKMANAGER_H
