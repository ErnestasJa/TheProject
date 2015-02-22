#ifndef VOXELMESH_H
#define VOXELMESH_H

#include "opengl/Mesh.h"

#include "Voxel.h"

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

class shader;
typedef std::shared_ptr<shader> shader_ptr;

class VoxelMesh:public Mesh
{
public:
    u16vec3 _size;
    bool _dirty;
private:
    void ClearBuffers();
public:
    BOOST_MOVABLE_BUT_NOT_COPYABLE(VoxelMesh);
    static Voxel EMPTY_VOXEL;
    VoxelMesh(const u16vec3 &size);
    virtual ~VoxelMesh();

    void CreateVox(int32_t x, int32_t y, int32_t z, const intRGBA &col);
    void RemoveVox(int32_t x, int32_t y, int32_t z);
    const Voxel& GetVoxel(int32_t x,int32_t y, int32_t z);

    void Render(bool wireframe=false);

    void UpdateMesh();

    void Cleanup();

    bool Empty();
protected:
    vector<Voxel> _vox;
    bool _empty;

    int32_t _faceCount;
    shader_ptr _shader;
private:
};
#endif // VOXELMESH_H

