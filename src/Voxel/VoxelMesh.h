#ifndef VOXELMESH_H
#define VOXELMESH_H

#include "OpenGL/Mesh.h"

#include "VoxelTypes.h"

#include "Voxel.h"

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

class shader;
typedef std::shared_ptr<shader> shader_ptr;

typedef boost::multi_array<Voxel, 3> VoxelArray;

static Voxel EMPTY_VOXEL=Voxel();

class VoxelMesh:Mesh
{
private:
    struct MaskNode
    {
        uint8_t exists;
        u8vec4 color;

        MaskNode & operator = (bool value)
        {
            exists = value;
            return *this;
        }

        operator bool()
        {
            return exists==1;
        }
    };
public:
    VoxelMesh(uint32_t size);
    virtual ~VoxelMesh();

    void CreateVox(int32_t x, int32_t y, int32_t z, const u8vec4 &col);

    void Render(bool wireframe=false);

    virtual void Rebuild()=0;

    void UpdateMesh();

    void Cleanup();

    bool Empty();

    uint32_t GetFaceCount();

protected:
    VoxelArray m_vox;

    bool m_dirty;

    int32_t m_size,m_faceCount;
    shader_ptr m_shader;

    uint32_t length(uint32_t x, uint32_t y, MaskNode **mask);
    uint32_t height(uint32_t x, uint32_t y, uint32_t len, MaskNode **mask);
    void clear_mask(MaskNode **mask);
    void clear_mask_ranged(MaskNode **mask,int sx,int sy,int ex,int ey);
    virtual void GetVoxel(Voxel &vox,int32_t x,int32_t y, int32_t z);
    void GreedyBuild();
    void AddQuadToMesh(const u8vec3 * face,const u8vec4 &col);
private:
};
#endif // VOXELMESH_H

