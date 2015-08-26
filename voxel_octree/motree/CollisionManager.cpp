#include "Precomp.h"
#include "utility/Vector.h"
#include "opengl/AABB.h"
#include "CollisionInfo.h"
#include "MortonOctree.h"
#include "CollisionManager.h"


CollisionManager::CollisionManager(MortonOctTreePtr octree)
{
    m_octree = octree;
    Depth = OCTREE_DEPTH;
}

CollisionManager::~CollisionManager()
{
    //dtor
}

bool CollisionManager::CheckCollision(const glm::vec3 & bmin, const glm::vec3 & bmax, const glm::vec3 & rayStart, const glm::vec3 & rayDirectionInverse)
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

bool CollisionManager::CheckCollision(const AABB & aabb)
{
	vector<MNode> & m_nodes = m_octree->GetChildNodes();

    auto clamp = [] (float & x)
    {
        if(x<0) x=0;
        else if(x>1023) x=1023;
    };
    auto clampVec = [&clamp] (glm::vec3 & x)
    {
        clamp(x.x);
        clamp(x.y);
        clamp(x.z);
    };

    glm::vec3 min = aabb.GetMin(),
              max = aabb.GetMax();

    clampVec(min);
    clampVec(max);

    uint32_t mortonMin = encodeMK(comp(min));
    uint32_t mortonMax = encodeMK(comp(max));

    auto low = boost::range::lower_bound(m_nodes,MNode(mortonMin));
    auto hi  = boost::range::lower_bound(m_nodes,MNode(mortonMax));

    if(hi!=m_nodes.end())
        hi++;

    uint32_t nodeCount=0;
    uint32_t x,y,z;
    for(; low != hi && low != m_nodes.end(); low++)
    {
        nodeCount++;
        decodeMK(low->start,x,y,z);
        if(aabb.IntersectsWith(glm::vec3(x,y,z),glm::vec3(x+1,y+1,z+1)))
        {
            std::cout<<"Nodes queried: " << nodeCount << std::endl;
            return true;
        }
    }

    //std::cout<<"Nodes queried: " << nodeCount << std::endl;
    return false;
}

bool CollisionManager::CheckCollisionB(const AABB & aabb)
{
	vector<MNode> & m_nodes = m_octree->GetChildNodes();

    auto clamp = [] (float & x)
    {
        if(x<0) x=0;
        else if(x>1023) x=1023;
    };
    auto clampVec = [&clamp] (glm::vec3 & x)
    {
        clamp(x.x);
        clamp(x.y);
        clamp(x.z);
    };

    glm::vec3 min = aabb.GetMin(),
              max = aabb.GetMax();

    clampVec(min);
    clampVec(max);
    
    for(uint32_t z = min.z; z < max.z; z++)
        for(uint32_t y = min.y; y < max.y; y++)
            for(uint32_t x = min.x; x < max.x; x++)
	      if(boost::range::binary_search(m_nodes,MNode(x,y,z)))
		return true;
	      
    return false;
}

static inline AABB BroadphaseAABB(const AABB & box, const glm::vec3 & vel)
{
  auto hvel = vel*0.5f;
  auto center = box.GetCenter()+hvel;
  auto size = box.GetHalfSize()+glm::abs(hvel);
  return AABB(center,size);
}

CollisionInfoVector CollisionManager::CheckCollisionSwept(const AABB & aabb, const glm::vec3 & vel)
{
    /*auto printAABB = [](const std::string & name, const AABB & bb)
    {
        glm::vec3 mi = bb.GetMin(),
        mx = bb.GetMax();
        printf("(%s) min [%0.2f,%0.2f,%0.2f], [%0.2f,%0.2f,%0.2f]]\n", name.c_str(), mi.x, mi.y, mi.z, mx.x, mx.y, mx.z);
    };*/


	vector<MNode> & m_nodes = m_octree->GetChildNodes();

    CollisionInfoVector infoVec;

    auto clamp = [] (float & x)
    {
        if(x<0)
            x=0;
        else if(x>1023)
            x=1023;
    };

    auto clampVec = [&clamp] (glm::vec3 & x)
    {
        clamp(x.x);
        clamp(x.y);
        clamp(x.z);
    };

    auto bpbox = BroadphaseAABB(aabb, vel);

    glm::vec3 min = bpbox.GetMin(),
    max = bpbox.GetMax();

    clampVec(min);
    clampVec(max);

    glm::vec3 normalOut;

    for(uint32_t z = min.z; z < max.z; z++)
    for(uint32_t y = min.y; y < max.y; y++)
    for(uint32_t x = min.x; x < max.x; x++)
        if(boost::range::binary_search(m_nodes,MNode(x,y,z)))
        {
            AABB b1(glm::vec3(x+0.5,y+0.5,z+0.5),glm::vec3(0.5,0.5,0.5));
            AABBCollisionInfo info;
            info.time = aabb.SweepCollidesWith(b1,vel,normalOut);

            if(info.time!=1.0f)
            {
                info.voxelMK = encodeMK(x,y,z);
                info.normal=normalOut;
                infoVec.push_back(info);
            }
        }
  return infoVec;
}

void CollisionManager::Collide(CollisionInfo & colInfo, uint32_t depthLevel, const glm::ivec3 & octStart)
{	
    glm::vec3 s(octStart.x,octStart.y,octStart.z);
    if(CheckCollision(s,s+glm::vec3(SIZE_TABLE[depthLevel]),colInfo.rayStart,colInfo.rayDirection))
    {
        if( depthLevel == Depth )
        {
            const float dist = glm::distance2(colInfo.rayStart,s);
            
            if(dist > 0 && dist < colInfo.nearestDistance && m_octree->CheckNode(octStart.x,octStart.y,octStart.z))
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

VoxelSide CollisionManager::GetCollisionSide(glm::vec3 voxPos, glm::vec3 rayStart,  glm::vec3 rayDirection)
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