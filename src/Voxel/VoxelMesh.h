#ifndef VOXELMESH_H
#define VOXELMESH_H

#include "VoxelTypes.h"

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

    void CreateVoxel(uint8_t x, uint8_t y, uint8_t z, uint32_t sides, u8vec4 color);
    void Render();
    virtual void Rebuild()=0;
    void UpdateMesh();
    void Cleanup();
    bool Empty();
protected:
    bool m_dirty;
    uint32_t m_indexTrack;
    uint32_t m_vertexTrack;
    MeshPtr m_mesh;
    shader_ptr m_shader;
    BufferObject<u8vec4> *m_colBuf;
    BufferObject<u8vec3> *m_posBuf;
    IndexBufferObject<uint32_t> *m_indBuf;
private:
};
#endif // VOXELMESH_H
