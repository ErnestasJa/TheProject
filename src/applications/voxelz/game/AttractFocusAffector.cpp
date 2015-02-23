#include "Precomp.h"
#include "AttractFocusAffector.h"
#include "opengl/AABB.h"
#include "ParticleSystem.h"

AttractFocusAffector::AttractFocusAffector(const glm::vec3 &attractionPoint,float power)
{
    _attractionPoint=attractionPoint;
    _power=power;
}

static glm::vec3 calc_force(glm::vec3 a, glm::vec3 b)
{
    glm::vec3 r = a-b;
    float r_mag = glm::length(r);
    glm::vec3 r_norm = glm::normalize(r);
    glm::vec3 ret = ((float)r_mag) * -r_norm;
    return ret;
}

void AttractFocusAffector::Affect(float dt,Particle &particleToAffect)
{
//    float dist=glm::distance(_attractionPoint,particleToAffect.pos);
//    glm::vec3 dir=glm::normalize(_attractionPoint-particleToAffect.pos);
    particleToAffect.pos+=calc_force(_attractionPoint,particleToAffect.pos)*dt;
}
