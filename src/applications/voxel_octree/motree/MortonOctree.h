#ifndef MORTONOCTTREE_H
#define	MORTONOCTTREE_H

#include "Morton.h"
#include "utility/Vector.h"
#include "opengl/AABB.h"

enum VoxelSide
{
    NONE = 0,
    ALL = bit<0>() | bit<1>() | bit<2>() | bit<3>() | bit<4>() | bit<5>(),
    TOP = bit<0>(),
    BOTTOM = bit<1>(),
    FRONT = bit<2>(),
    RIGHT = bit<3>(),
    BACK = bit<4>(),
    LEFT = bit<5>()
};

static const uint8_t MAX_DEPTH_LEVEL = 10;
static const uint32_t DEPTH_HALF_TABLE[]= {0,1,2,4,8,16,32,64,128,256,512,1024};    ///HALF NODE COUNT AT LEVEL X
static const uint32_t SIZE_TABLE[]=       {1024,512,256,128,64,32,16,8,4,2,1};      ///NODE SIZE AT LEVEL X
static const uint32_t DEPTH_TABLE[]=      {2,4,8,16,32,64,128,256,512,1024};        ///NODE COUNT AT LEVEL X
static const uint32_t POSITION_MASK[]= {0xfffffe00,0xffffff00,0xffffff80,0xffffffc0,0xffffffe0,0xfffffff0,0xfffffff8,0xfffffffc,0xfffffffe,0xffffffff}; /// Don't question the hax
static const uint32_t CHUNK_MASK= ~0x7FFF;
static const uint32_t LOCAL_VOXEL_MASK= 0x7FFF;

#include "MNode.h"

struct CollisionInfo
{
    CollisionInfo(glm::vec3 ray_start, glm::vec3 ray_direction)
    {
        nearestDistance = INFINITY;
        rayStart = ray_start;
        rayDirection = ray_direction;
        helpers::invert(rayDirection);
    }

    MNode node;
    float nearestDistance;
    glm::vec3 rayStart, rayDirection;
};

struct AABBCollisionInfo
{
    uint32_t voxelMK;
    float time;
    glm::vec3 normal;
};

typedef std::vector<AABBCollisionInfo> CollisionInfoVector;

template <int Depth>
class MortonOctTree
{
public:
    void AddNode(MNode node);
    void AddOrphanNode(MNode node);
    bool IsSorted();
    void SortLeafNodes();
    void RemoveDuplicateNodes();
    inline bool CheckNodeFloat(float x, float y, float z);
    inline bool CheckNode(uint32_t x, uint32_t y, uint32_t z);
    uint8_t GetVisibleSides(uint32_t x, uint32_t y, uint32_t  z, vector<MNode>::iterator nodeIt);
    bool CheckCollision(const glm::vec3 & bmin, const glm::vec3 & bmax, const glm::vec3 & rayStart, const glm::vec3 & rayDirectionInverse);
    bool CheckCollision(const AABB & aabb);
    bool CheckCollisionB(const AABB & aabb);
    CollisionInfoVector CheckCollisionSwept(const AABB & aabb, const glm::vec3 & vel);
    void Collide(CollisionInfo & colInfo, uint32_t depthLevel, const glm::ivec3 & octStart);
    VoxelSide GetCollisionSide(glm::vec3 voxPos, glm::vec3 rayStart,  glm::vec3 rayDirection);
    glm::ivec3 VoxelSideToPosition(VoxelSide side);

    vector<MNode> & GetChildNodes();
private:
    vector<MNode> m_nodes;
};

typedef std::shared_ptr<MortonOctTree<10>> MortonOctTreePtr;

#include "MortonOctree.inl"

#endif	/* MORTONOCTTREE_H */
