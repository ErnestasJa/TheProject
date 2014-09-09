#ifndef MORTONOCTTREE_H
#define	MORTONOCTTREE_H

#include "Morton.h"

#include <assert.h>
#include <stdint.h>
#include <vector>
#include <algorithm>

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

struct MNode {
    int64_t start;
    unsigned contained_by : 24;
    unsigned size : 8;
    int object_id;

    MNode()
    {
        start = 0;
        object_id = 0;
        size = 0;
        contained_by = 0;
    }

    MNode(uint32_t x, uint32_t y, uint32_t z, uint8_t nodeSize=1)
    {
        start = encodeMortonKey(x,y,z);
        size = nodeSize;
        object_id = 0;
        size = 0;
        contained_by = 0;
    }

    inline int64_t range_end() const {
        return start + (int64_t(1) << (size * 3));
    }

    bool operator<(const MNode &other) const {/// ordering: z order, plus on equal sizes largest first
        return start == other.start ? size > other.size : start < other.start;
    }

    bool operator==(const MNode &other) const {
        return start == other.start && size == other.size;
    }
};

static const uint8_t MAX_DEPTH_LEVEL = 16;
static const uint32_t DEPTH_HALF_TABLE[]={0,1,2,4,8,16,32,64,128,256,512,2048,4096,8192,16384,32768};    ///HALF NODE COUNT AT LEVEL X
static const uint32_t DEPTH_TABLE[]=     {1,2,4,8,16,32,64,128,256,512,1024,2048,4096,8192,16384,32768,65536}; ///NODE COUNT AT LEVEL X
static const uint32_t POSITION_MASK[]=   {0,1,3,7,15,31,63,127,255,511,1023,2047,4095,8191,16383,32767,65535};

template <int Depth>
class MortonOctTree
{
public:

    void AddNode(const MNode & node)
    {
        m_nodes[Depth-1].push_back(node);
    }

    //sort and build tree, ...
    void RefineTree()
    {
        std::sort(m_nodes[Depth-1].begin(), m_nodes[Depth-1].end());

        for(int i = 0; i < Depth-1; i++)
            m_nodes[i].clear();

        for(auto node : m_nodes[Depth-1])
            PopulateParents(node);

        for(int i = 0; i < Depth-1; i++)
            std::sort(m_nodes[i].begin(), m_nodes[i].end());
    }

    void PopulateParents(const MNode & node)
    {
        uint32_t x,y,z;
        decodeMortonKey(node.start, x, y, z);

        for( int i = Depth - 2; i > -1; i--)
        {
            x &= POSITION_MASK[i];
            y &= POSITION_MASK[i];
            z &= POSITION_MASK[i];

            MNode parent = MNode(x,y,z,Depth-i);

            auto found = std::find(m_nodes[i].begin(), m_nodes[i].end(), parent);

            if( found ==m_nodes[i].end() )
                m_nodes[i].push_back(parent);
        }
    }

    bool CheckNode(float x, float y, float z)
    {
        return false;
    }

    std::vector<MNode> & GetChildNodes()
    {
        return m_nodes[Depth-1];
    }

    bool CheckCollision(const glm::vec3 & bmin, const glm::vec3 & bmax, glm::vec3 rayStart,  glm::vec3 rayDirectionInverse);
    bool Collide(MNode & node, glm::vec3 rayStart,  glm::vec3 rayDirection);
    VoxelSide GetCollisionSide(glm::vec3 voxPos, glm::vec3 rayStart,  glm::vec3 rayDirection);
    glm::ivec3 VoxelSideToPosition(VoxelSide side);

    friend class VoxMeshGenerator;

private:
    std::vector<MNode> m_nodes[Depth];
};

template <int Depth>
bool MortonOctTree<Depth>::CheckCollision(const glm::vec3 & bmin, const glm::vec3 & bmax, glm::vec3 rayStart,  glm::vec3 rayDirectionInverse)
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
bool MortonOctTree<Depth>::Collide(MNode & node, glm::vec3 rayStart,  glm::vec3 rayDirection)
{
    if(rayDirection.x)
        rayDirection.x = 1.0f / rayDirection.x;
    if(rayDirection.y)
        rayDirection.y = 1.0f / rayDirection.y;
    if(rayDirection.z)
        rayDirection.z = 1.0f / rayDirection.z;

    float nearestDistance = INFINITY;
    float distance;

    for(int i = 0; i < Depth; i++)
    {
        bool collided=false;
        uint32_t size = DEPTH_TABLE[(Depth-1)-i];

        for(MNode & n : m_nodes[i])
        {
            uint32_t x,y,z;
            decodeMortonKey(n.start,x,y,z);
            glm::vec3   vox_start(x,y,z),
                        vox_end(x+size,y+size,z+size);

            if(CheckCollision(vox_start,vox_end,rayStart,rayDirection))
            {
                collided = true;
                if(i==Depth-1)
                {
                    distance = glm::distance2(vox_start,rayStart); /// could do this earlier to avoid collision checks.

                    if( distance < nearestDistance)
                    {
                        nearestDistance = distance;
                        node = n;
                    }
                }
            }
        }

        if(collided == false)
            return false;
    }

    return true;
}
#include "utility/helpers.h"
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
    }

    return pos;
}

#endif	/* MORTONOCTTREE_H */

