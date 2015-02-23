#include "Precomp.h"

#include "GravityAffector.h"

GravityAffector::~GravityAffector()
{
    //dtor
}

void GravityAffector::Affect(float dt,Particle &particleToAffect)
{
    particleToAffect.speed+=glm::vec3(0.f,-9.81f,0.f)*dt;
}
