#ifndef COLLISIONMANAGER_H
#define COLLISIONMANAGER_H

#include "utility/Vector.h"

class AABB;
struct CollisionInfo;
struct AABBCollisionInfo;
typedef vector<AABBCollisionInfo> CollisionInfoVector;
class MortonOctTree;
typedef std::shared_ptr<MortonOctTree> MortonOctTreePtr;

#include "VoxelSide.h"

class CollisionManager
{
    public:
        CollisionManager(MortonOctTreePtr octree);
        virtual ~CollisionManager();

    	bool CheckCollision(const glm::vec3 & bmin, const glm::vec3 & bmax, const glm::vec3 & rayStart, const glm::vec3 & rayDirectionInverse);
    	bool CheckCollision(const AABB & aabb);
    	bool CheckCollisionB(const AABB & aabb);
    	CollisionInfoVector CheckCollisionSwept(const AABB & aabb, const glm::vec3 & vel);
    	void Collide(CollisionInfo & colInfo, uint32_t depthLevel, const glm::ivec3 & octStart);
    	VoxelSide GetCollisionSide(glm::vec3 voxPos, glm::vec3 rayStart,  glm::vec3 rayDirection);

    protected:
        MortonOctTreePtr m_octree;
        uint32_t Depth; /// just until we get rid of templated octree.
};

#endif // COLLISIONMANAGER_H
