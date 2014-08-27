#ifndef VOXELMESH_H
#define VOXELMESH_H

enum EBlockSides
{
    EBS_FRONT=BIT0,
    EBS_BACK=BIT1,
    EBS_TOP=BIT2,
    EBS_BOTTOM=BIT3,
    EBS_LEFT=BIT4,
    EBS_RIGHT=BIT5,

    EBS_COUNT
};

class Mesh;
typedef std::shared_ptr<Mesh> MeshPtr;

class shader;
typedef std::shared_ptr<shader> shader_ptr;

template<typename T>
class BufferObject;

template<typename T>
class IndexBufferObject;

class VoxelMesh
{
public:
    VoxelMesh();
    virtual ~VoxelMesh();

    void CreateVoxel(float x, float y, float z, uint32_t sides, glm::vec4 color);
    void Render();
    void Rebuild();
    void Cleanup();
protected:
    bool m_dirty;
    uint32_t m_indexTrack;
    MeshPtr m_mesh;
    shader_ptr m_shader;
    BufferObject<glm::vec4> *m_colBuf;
    BufferObject<glm::vec3> *m_posBuf;
    IndexBufferObject<uint32_t> *m_indBuf;
private:
};
#endif // VOXELMESH_H
