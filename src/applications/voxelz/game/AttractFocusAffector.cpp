#include "Precomp.h"
#include "AttractFocusAffector.h"
#include "opengl/AABB.h"
#include "ParticleSystem.h"

AttractFocusAffector::AttractFocusAffector(const glm::vec3 &attractionPoint,float power)
{
    _attractionPoint=attractionPoint;
    _power=power;
}

void AttractFocusAffector::Affect(float dt,Particle &particleToAffect)
{
    float dist=glm::distance(_attractionPoint,particleToAffect.pos);
    glm::vec3 dir=glm::normalize(_attractionPoint-particleToAffect.pos);
    particleToAffect.pos+=particleToAffect.speed*_power*dt*-dir*200.f/dist;
}
