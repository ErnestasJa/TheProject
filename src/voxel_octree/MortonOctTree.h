#ifndef MORTONOCTTREE_H
#define	MORTONOCTTREE_H

#include "Morton.h"
#include "utility/vector.h"
#include <boost/move/move.hpp>

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

struct MNode
{
private:
    BOOST_MOVABLE_BUT_NOT_COPYABLE(MNode)

public:

    uint32_t start;
    uint32_t contained_by : 24;
    uint32_t size : 8;

    MNode(BOOST_RV_REF(MNode) n): start(n.start), size(n.size) {}

    MNode& operator=(BOOST_RV_REF(MNode) x) // Move assign
    {
        start = x.start;
        size  = x.size;
        return *this;
    }

    MNode(uint32_t x, uint32_t y, uint32_t z, uint8_t nodeSize=1)
    {
        start = encodeMK(x,y,z);
        size = nodeSize;
        contained_by = 0;
    }

    MNode(uint32_t morton, uint8_t nodeSize=1)
    {
        start = morton;
        size = nodeSize;
        contained_by = 0;
    }

    MNode()
    {
        size = 0;
    }

    inline uint32_t range_end() const
    {
        return start + (uint32_t(1) << (size * 3));
    }

    bool operator<(const MNode &other) const  /// ordering: z order, plus on equal sizes largest first
    {
        return start == other.start ? size > other.size : start < other.start;
    }

    bool operator==(const MNode &other) const
    {
        return start == other.start && size == other.size;
    }
};

static inline bool myfunction (const MNode & i,const MNode & j)
{
    return (i.start<j.start);
}

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

template <int Depth>
class MortonOctTree
{
public:
    void AddNode(MNode node)
    {
        m_nodes.insert(boost::range::lower_bound(m_nodes, MNode(node.start)),boost::move(node));
    }

    void AddOrphanNode(MNode node)
    {
        m_nodes.push_back(boost::move(node));
    }

    bool IsSorted()
    {
        return boost::range::is_sorted(m_nodes, myfunction);
    }

    void SortLeafNodes()
    {
        boost::range::sort(m_nodes, myfunction);
    }

    inline bool CheckNodeFloat(float x, float y, float z)
    {
        if(x<0||y<0||z<0)
            return false;

        return CheckNode(x,y,z);
    }

    inline bool CheckNode(uint32_t x, uint32_t y, uint32_t z)
    {
        MNode n(x,y,z,1);
        return boost::range::binary_search(boost::make_iterator_range(m_nodes.begin(),m_nodes.end()),n,myfunction);
    }

    uint8_t GetVisibleSides(uint32_t x, uint32_t y, uint32_t  z, vector<MNode>::iterator nodeIt)
    {
        uint8_t sides=ALL;

        static MNode n;
        n.size = 1;

        auto urange = boost::make_iterator_range(nodeIt,m_nodes.end());
        auto lrange = boost::make_iterator_range(m_nodes.begin(),nodeIt);

        n.start = encodeMK(x,y+1,z);
        if (boost::range::binary_search(urange,n,myfunction))
            RemoveBit(sides, TOP);

        n.start = encodeMK(x,y,z+1);
        if (boost::range::binary_search(urange,n,myfunction))
            RemoveBit(sides, FRONT);

        n.start = encodeMK(x+1,y,z);
        if (boost::range::binary_search(urange,n,myfunction))
            RemoveBit(sides, LEFT);

        n.start = encodeMK(x-1,y,z);
        if (boost::range::binary_search(lrange,n,myfunction))
            RemoveBit(sides, RIGHT);

        n.start = encodeMK(x,y,z-1);
        if (boost::range::binary_search(lrange,n,myfunction))
            RemoveBit(sides, BACK);

        n.start = encodeMK(x,y-1,z);
        if (boost::range::binary_search(lrange,n,myfunction))
            RemoveBit(sides, BOTTOM);

        return sides;
    }

    vector<MNode> & GetChildNodes()
    {
        return m_nodes;
    }

    bool CheckCollision(const glm::vec3 & bmin, const glm::vec3 & bmax, const glm::vec3 & rayStart, const glm::vec3 & rayDirectionInverse);
    void Collide(CollisionInfo & colInfo, uint32_t depthLevel, const glm::ivec3 & octStart);
    VoxelSide GetCollisionSide(glm::vec3 voxPos, glm::vec3 rayStart,  glm::vec3 rayDirection);
    glm::ivec3 VoxelSideToPosition(VoxelSide side);

    friend class VoxMeshManager;
private:
    vector<MNode> m_nodes;
};

template <int Depth>
bool MortonOctTree<Depth>::CheckCollision(const glm::vec3 & bmin, const glm::vec3 & bmax, const glm::vec3 & rayStart, const glm::vec3 & rayDirectionInverse)
{
    double tx1 = (bmin.x - rayStart.x)*rayDirectionInverse.x;
    double tx2 = (bmax.x - rayStart.x)*rayDirectionInverse.x;

    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);

    double ty1 = (bmin.y - rayStart.y)*rayDirectionInverse.y;
    double ty2 = (bmax.y - rayStart.y)*rayDirectionInverse.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    double tz1 = (bmin.z - rayStart.z)*rayDirectionInverse.z;
    double tz2 = (bmax.z - rayStart.z)*rayDirectionInverse.z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= tmin;
}

template <int Depth>
void MortonOctTree<Depth>::Collide(CollisionInfo & colInfo, uint32_t depthLevel, const glm::ivec3 & octStart)
{
    glm::vec3 s(octStart.x,octStart.y,octStart.z);
    if(CheckCollision(s,s+glm::vec3(SIZE_TABLE[depthLevel]),colInfo.rayStart,colInfo.rayDirection))
    {
        if( depthLevel == Depth )
        {
            const float dist = glm::distance2(colInfo.rayStart,s);
            if(dist < colInfo.nearestDistance && CheckNode(octStart.x,octStart.y,octStart.z))
            {
                colInfo.nearestDistance = dist;
                colInfo.node.start = encodeMK(octStart.x,octStart.y,octStart.z);
                colInfo.node.size = 1;
            }
            return;
        }

        depthLevel+=1;
        const uint32_t size = SIZE_TABLE[depthLevel];
        Collide(colInfo,depthLevel,octStart);
        Collide(colInfo,depthLevel,octStart+glm::ivec3(size,0,0));
        Collide(colInfo,depthLevel,octStart+glm::ivec3(size,0,size));
        Collide(colInfo,depthLevel,octStart+glm::ivec3(0,0,size));

        Collide(colInfo,depthLevel,octStart+glm::ivec3(0,size,0));
        Collide(colInfo,depthLevel,octStart+glm::ivec3(size,size,0));
        Collide(colInfo,depthLevel,octStart+glm::ivec3(size,size,size));
        Collide(colInfo,depthLevel,octStart+glm::ivec3(0,size,size));
    }

    return;
}

/**
template <int Depth>
bool MortonOctTree<Depth>::Collide(MNode & node, uint32_t size, const glm::vec3 & octStart, const glm::vec3 & rayStart, const glm::vec3 & rayDirection)
{

    float nearestDistance = INFINITY;
    float distance;

    for(int i = 0; i < Depth; i++)
    {
        bool collided=false;
        uint32_t size = SIZE_TABLE[(Depth-1)-i];

        for(MNode & n : m_nodes)
        {
            uint32_t x,y,z;
            decodeMK(n.start,x,y,z);
            glm::vec3 vox_start(x,y,z), vox_end(x+size,y+size,z+size);

            if(CheckCollision(vox_start,vox_end,rayStart,rayDirection))
            {
                collided = true;
                if(i==Depth-1)
                {
                    distance = glm::distance2(vox_start,rayStart); /// could do this earlier to avoid collision checks.

                    if( distance < nearestDistance)
                    {
                        nearestDistance = distance;
                        node.start =  n.start;
                    }
                }
            }
        }

        if(collided == false)
            return false;
    }

    return true;
}
**/

static bool IsRayIntersectingTriangle(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 normal, glm::vec3 rayStart,  glm::vec3 rayDirection)
{
    if(glm::dot(normal, rayDirection)>0.0f)
        return false;

    glm::vec3 ab(b-a), ac(c-a);
    glm::vec3 P, Q, T;
    float det, inv_det, u, v, t;

    P = glm::cross(rayDirection, ac);
    det = glm::dot(ab,P);

    if(helpers::equals(det,0.0f))
        return false;

    inv_det = 1.f / det;

    T = rayStart - a;
    u = glm::dot(T, P) * inv_det;

    if(u < 0.f || u > 1.f) return false;

    Q = glm::cross(T, ab);
    v = glm::dot(rayDirection, Q) * inv_det;
    if(v < 0.f || u + v  > 1.f) return 0;

    t = glm::dot(ac, Q) * inv_det;

    return t > ROUNDING_ERROR;
}

template <int Depth>
VoxelSide MortonOctTree<Depth>::GetCollisionSide(glm::vec3 voxPos, glm::vec3 rayStart,  glm::vec3 rayDirection)
{
    static glm::vec3 up(0,1,0), down(0,-1,0), front(0,0,1), back(0,0,-1), left(-1,0,0), right(1,0,0);

    VoxelSide side = NONE;

    if(IsRayIntersectingTriangle(voxPos+glm::vec3(0,1,0),voxPos+glm::vec3(1,1,0), voxPos+glm::vec3(0,1,1), up, rayStart, rayDirection)||
            IsRayIntersectingTriangle(voxPos+glm::vec3(1,1,1),voxPos+glm::vec3(1,1,0), voxPos+glm::vec3(0,1,1), up, rayStart, rayDirection))
    {
        //std::cout << "Collided with voxel top" << std::endl;
        side = TOP;
    }
    else if(IsRayIntersectingTriangle(voxPos,voxPos+glm::vec3(1,0,0), voxPos+glm::vec3(0,0,1), down, rayStart, rayDirection)||
            IsRayIntersectingTriangle(voxPos+glm::vec3(1,0,1),voxPos+glm::vec3(1,0,0), voxPos+glm::vec3(0,0,1), down, rayStart, rayDirection))
    {
        //std::cout << "Collided with voxel bottom" << std::endl;
        side = BOTTOM;
    }
    else if(IsRayIntersectingTriangle(voxPos+glm::vec3(0,0,0),voxPos+glm::vec3(1,0,0), voxPos+glm::vec3(0,1,0), back, rayStart, rayDirection)||
            IsRayIntersectingTriangle(voxPos+glm::vec3(0,1,0),voxPos+glm::vec3(1,0,0), voxPos+glm::vec3(1,1,0), back, rayStart, rayDirection))
    {
        //std::cout << "Collided with voxel back" << std::endl;
        side = BACK;
    }
    else if(IsRayIntersectingTriangle(voxPos+glm::vec3(0,0,1),voxPos+glm::vec3(1,0,1), voxPos+glm::vec3(0,1,1), front, rayStart, rayDirection)||
            IsRayIntersectingTriangle(voxPos+glm::vec3(0,1,1),voxPos+glm::vec3(1,0,1), voxPos+glm::vec3(1,1,1), front, rayStart, rayDirection))
    {
        //std::cout << "Collided with voxel front" << std::endl;
        side = FRONT;
    }
    else if(IsRayIntersectingTriangle(voxPos+glm::vec3(0,0,0),voxPos+glm::vec3(0,0,1), voxPos+glm::vec3(0,1,0), left, rayStart, rayDirection)||
            IsRayIntersectingTriangle(voxPos+glm::vec3(0,1,1),voxPos+glm::vec3(0,0,1), voxPos+glm::vec3(0,1,0), left, rayStart, rayDirection))
    {
        //std::cout << "Collided with voxel left" << std::endl;
        side = LEFT;
    }
    else if(IsRayIntersectingTriangle(voxPos+glm::vec3(1,0,0),voxPos+glm::vec3(1,0,1), voxPos+glm::vec3(1,1,0), right, rayStart, rayDirection)||
            IsRayIntersectingTriangle(voxPos+glm::vec3(1,1,1),voxPos+glm::vec3(1,0,1), voxPos+glm::vec3(1,1,0), right, rayStart, rayDirection))
    {
        //std::cout << "Collided with voxel right" << std::endl;
        side = RIGHT;
    }

    return side;
}

template <int Depth>
glm::ivec3 MortonOctTree<Depth>::VoxelSideToPosition(VoxelSide side)
{
    glm::ivec3 pos;

    switch(side)
    {
    case TOP:
        pos.y=1;
        break;
    case BOTTOM:
        pos.y=-1;
        break;
    case RIGHT:
        pos.x=1;
        break;
    case LEFT:
        pos.x=-1;
        break;
    case FRONT:
        pos.z=1;
        break;
    case BACK:
        pos.z=-1;
        break;
    default:
        break;
    }

    return pos;
}

#endif	/* MORTONOCTTREE_H */

