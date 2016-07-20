#ifndef COLLISION_INFO_H
#define COLLISION_INFO_H

#include "utility/Vector.h"
#include "MNode.h"

struct CollisionInfo
{
    CollisionInfo(glm::vec3 ray_start, glm::vec3 ray_direction);
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

typedef vector<AABBCollisionInfo> CollisionInfoVector;

#endif