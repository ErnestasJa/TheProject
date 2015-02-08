#ifndef VOXELMESH_H
#define VOXELMESH_H

#include "opengl/Mesh.h"

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

class VoxelMesh:public Mesh
{
private:
    struct MaskNode
    {
        BOOST_MOVABLE_BUT_NOT_COPYABLE(MaskNode)

        uint8_t exists;
        intRGBA color;

        MaskNode()
        {
            exists=false;
            color=VecRGBToIntRGB(u8vec3(255));
        }

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
    BOOST_MOVABLE_BUT_NOT_COPYABLE(VoxelMesh);
    static Voxel EMPTY_VOXEL;
    VoxelMesh(uint32_t size,bool init=false);
    virtual ~VoxelMesh();

    void CreateVox(int32_t x, int32_t y, int32_t z, const intRGBA &col);
    void RemoveVox(int32_t x, int32_t y, int32_t z);

    void Render(bool wireframe=false);

    virtual void Rebuild(){};

    void UpdateMesh();

    void Cleanup();

    bool Empty();

    uint32_t GetFaceCount();

    void GreedyBuild();

protected:
    vector<Voxel> m_vox;

    bool m_dirty;
    bool m_empty;

    int32_t m_size,m_faceCount,mVecTrack,mIndexTrack;
    shader_ptr m_shader;

    uint32_t length(uint32_t x, uint32_t y, MaskNode **mask);
    uint32_t height(uint32_t x, uint32_t y, uint32_t len, MaskNode **mask);
    void clear_mask(MaskNode **mask);
    void clear_mask_ranged(MaskNode **mask,int sx,int sy,int ex,int ey);
    virtual const Voxel& GetVoxel(int32_t x,int32_t y, int32_t z);
    virtual void AddQuadToMesh(const glm::ivec3 * face,const intRGBA &col);
    void ClearBuffers();
private:
};
#endif // VOXELMESH_H

