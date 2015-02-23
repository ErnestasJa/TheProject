#ifndef ATTRACTFOCUSAFFECTOR_H
#define ATTRACTFOCUSAFFECTOR_H

#include "IParticleAffector.h"

class AttractFocusAffector:public IParticleAffector
{
private:
    glm::vec3 _attractionPoint;
    float _power;

public:
    AttractFocusAffector(const glm::vec3 &attractionPoint,float power);
    virtual ~AttractFocusAffector(){};
    void Affect(float dt,Particle & particleToAffect, ParticleEmitter *emitter);
protected:
private:
    glm::vec3 calc_force(Particle &p, const glm::vec3 &attractionPoint);
};

#endif // ATTRACTFOCUSAFFECTOR_H
