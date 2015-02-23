#include "Precomp.h"

#include "opengl/AABB.h"

#include "AttractFocusAffector.h"
#include "ParticleEmitter.h"

AttractFocusAffector::AttractFocusAffector(const glm::vec3 &attractionPoint,float power)
{
    _attractionPoint=attractionPoint;
    _power=power;
}

glm::vec3 AttractFocusAffector::calc_force(Particle &p, const glm::vec3 &attractionPoint)
{
    float m=p.mass;

    glm::vec3 r = p.pos-attractionPoint;
    float r_mag = _power;
    glm::vec3 r_norm = glm::normalize(r);
    glm::vec3 ret = (-(float)(666.f)*m/(float)pow(r_mag,2.0f)) * r_norm;
    return ret;
}

void AttractFocusAffector::Affect(float dt,Particle & particleToAffect, ParticleEmitter *emitter)
{
//    float dist=glm::distance(_attractionPoint,particleToAffect.pos);
//    glm::vec3 dir=glm::normalize(_attractionPoint-particleToAffect.pos);
    particleToAffect.speed+=calc_force(particleToAffect,_attractionPoint)*dt;
    //if(glm::distance(particleToAffect.pos,_attractionPoint)<_power) particleToAffect.life=0.f;
}
