#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "Voxel/VoxelTypes.h"

struct Particle
{
    glm::vec3 pos,speed;
    u8vec4 col;

    float size,angle,weight;
    float life;
    float mass;

    Particle()
    {
        life=0;
        size=angle=weight=0;
        col=u8vec4(255);
        pos=speed=glm::vec3(0);
    }
};

#endif // PARTICLE_H_INCLUDED
