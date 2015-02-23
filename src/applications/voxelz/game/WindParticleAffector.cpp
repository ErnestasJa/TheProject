#include "Precomp.h"

#include "WindParticleAffector.h"
#include "opengl/AABB.h"

WindParticleAffector::WindParticleAffector(const AABB &windBox,const glm::vec3 & direction,float speed)
{
    _windBox=windBox;
    _direction=direction;
    _speed=speed;
}

void WindParticleAffector::Affect(float dt,Particle & particleToAffect, ParticleEmitter *emitter)
{
    if(_windBox.ContainsPoint(particleToAffect.pos))
        particleToAffect.speed+=(_direction+glm::vec3((float)(rand()%1000)/10000.f,(float)(rand()%1000)/10000.f,(float)(rand()%1000)/10000.f))*_speed*dt;
}
