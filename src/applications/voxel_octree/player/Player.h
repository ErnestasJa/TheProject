#ifndef PLAYER_H
#define PLAYER_H

#include "scenegraph/Camera.h"
#include "voxel_octree/MortonOctree.h"

static glm::vec3 gGravity = glm::vec3(0,1,0);

class Player
{
    public:
        Player(CameraPtr cam, MortonOctTreePtr octree, glm::vec3 position, AABB aabb, glm::vec3 eyeOffset = glm::vec3(0,1.5,0));
        virtual ~Player();

        glm::vec3 & GetPosition();
        glm::vec3 & GetEyeOffset();
        CameraPtr GetCamera();

        void Update(float timeStep);
        void ApplyGravity(float timeStep);

    protected:
        MortonOctTreePtr m_octree;
        CameraPtr m_cam;
        glm::vec3   m_eyeOffset,
                    m_position,
                    m_velocity;
        AABB m_aabb;
};

#endif // PLAYER_H
