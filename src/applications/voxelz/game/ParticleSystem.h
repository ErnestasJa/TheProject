#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

#define MAX_PARTICLES 1000000

#include "Voxel/VoxelTypes.h"
#include "Opengl/BufferObject.h"

struct Particle
{
    glm::vec3 pos,speed;
    u8vec4 col;
    float size,angle,weight;
    float life;
};

class OpenGLUtil;

class ParticleSystem
{
public:
    BufferObject<glm::vec3> *_geometry;
    BufferObject<glm::vec3> *_pos;
    BufferObject<u8vec4> *_col;

    uint32_t _VAO;

private:
    Particle _particlesContainer[MAX_PARTICLES];
    uint32_t _lastUsedParticle;
    uint32_t _particleCount;
protected:

public:
    ParticleSystem();
    virtual ~ParticleSystem();

    void Render(OpenGLUtil *util);
    void Update(float dt);

    uint32_t FindUnused()
    {
        loopr(i,_lastUsedParticle,MAX_PARTICLES)
        {
            if(_particlesContainer[i].life<=0)
            {
                _lastUsedParticle=i;
                return i;
            }
        }

        loop(i,_lastUsedParticle)
        {
            if(_particlesContainer[i].life<=0)
            {
                _lastUsedParticle=i;
                return i;
            }
        }

        return 0;
    }
protected:
private:
};

#endif // PARTICLESYSTEM_H
