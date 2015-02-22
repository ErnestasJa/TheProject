#ifndef PARTICLEEMITTER_H
#define PARTICLEEMITTER_H

#include "Voxel/VoxelTypes.h"
#include "utility/Vector.h"
#include "Opengl/BufferObject.h"


class IParticleAffector;
class Particle;

class ParticleEmitter
{
public:
    glm::vec3 _pos;
    glm::vec3 _direction;

    float _speed;
    float _spread;

    float _particleLife;
    float _particleSize;

    uint32_t _particleCount,_deadParticles,_maxParticles,_lastUsedParticle;

    bool _loop;
    bool _finished;

    vector<Particle> _particleContainer;
private:
    vector<IParticleAffector*> _particleAffectors;
protected:
public:
    ParticleEmitter(glm::vec3 pos,glm::vec3 direction, float speed, float spread,float particleLife,uint32_t maxParticles=1024,bool loop=true);

    virtual ~ParticleEmitter();

    void Update(float dt,uint32_t &particleCount, BufferObject<glm::vec3> *pos, BufferObject<u8vec4> *col);

    void AddParticleAffector(IParticleAffector* affector);

    uint32_t FindUnused();
private:

protected:
};

#endif // PARTICLEEMITTER_H
