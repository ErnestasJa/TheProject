#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#define MAX_PARTICLES 100000

#include "Voxel/VoxelTypes.h"
#include "Opengl/BufferObject.h"

struct Particle
{
    glm::vec3 pos,speed;
    u8vec4 col;
    float size,angle,weight;
    float life;
    Particle()
    {
        life=0;
        size=angle=weight=0;
        col=u8vec4(255);
        pos=speed=glm::vec3(0);
    }
};

class ParticleEmitter;

class ParticleSystem
{
public:
    BufferObject<glm::vec3> *_geometry;
    BufferObject<glm::vec3> *_pos;
    BufferObject<u8vec4> *_col;

    uint32_t _VAO;

private:
    Particle _particlesContainer[MAX_PARTICLES];
    vector<ParticleEmitter*> _emitters;
    uint32_t _lastUsedParticle;
    uint32_t _particleCount;
    float _rotate;
protected:

public:
    ParticleSystem();
    virtual ~ParticleSystem();

    void Render();
    void Update(float dt);

    uint32_t FindUnused();

    void AddEmitter(ParticleEmitter* emitter);
protected:
private:
    void BuildParticle(Particle& p,ParticleEmitter* e);
};

#endif // PARTICLESYSTEM_H
