#ifndef CHUNK_H
#define CHUNK_H

#include "Block.h"

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

class shader;
typedef std::shared_ptr<shader> shader_ptr;

template<typename T>
class BufferObject;

template<typename T>
class IndexBufferObject;

class Chunk
{
public:
    Chunk();
    ~Chunk();

    void SetupSphere();
    void Generate();

    void Update(float dt);

    void Render();

    static const int CHUNK_SIZE = 16;

private:
    MeshPtr m_mesh;
    shader_ptr m_shader;
    BufferObject<glm::vec4> *m_colBuf;
    BufferObject<glm::vec3> *m_posBuf;
    IndexBufferObject<uint32_t> *m_indBuf;

    // The blocks data
    Block*** m_pBlocks;

    void CreateCube(float x,float y,float z);
};
#endif // CHUNK_H