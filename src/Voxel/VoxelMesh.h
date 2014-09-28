#ifndef VOXELMESH_H
#define VOXELMESH_H

#include "Voxel.h"
#include "VoxelTypes.h"

#include <boost/multi_array.hpp>

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

typedef boost::multi_array<Voxel, 3> VoxelArray;

class VoxelMesh
{
public:
    VoxelMesh(uint8_t xsize,uint8_t ysize,uint8_t zsize);
    virtual ~VoxelMesh();

    void CreateVox(uint8_t x, uint8_t y, uint8_t z);
    void CreateVoxel(uint8_t x, uint8_t y, uint8_t z, uint32_t sides, u8vec4 color);
    void Render();
    virtual void Rebuild()=0;
    void UpdateMesh();
    void Cleanup();
    bool Empty();
protected:
    VoxelArray m_vox;
    bool m_dirty;
    uint8_t m_xsize,m_ysize,m_zsize;
    uint32_t m_indexTrack;
    uint32_t m_vertexTrack;
    MeshPtr m_mesh;
    shader_ptr m_shader;
    BufferObject<u8vec4> *m_colBuf;
    BufferObject<u8vec3> *m_posBuf;
    IndexBufferObject<uint32_t> *m_indBuf;
private:

    void GetBuildNode(Voxel &vox,uint8_t x,uint8_t y, uint8_t z);
    void GreedyBuild(const u8vec3 & offset);
    void AddQuadToMesh(const u8vec3 * face);
};
#endif // VOXELMESH_H
