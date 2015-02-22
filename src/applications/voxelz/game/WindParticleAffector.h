#ifndef WINDPARTICLEAFFECTOR_H
#define WINDPARTICLEAFFECTOR_H

#include "IParticleAffector.h"

class AABB;

class WindParticleAffector:public IParticleAffector
{
    public:
        WindParticleAffector(const AABB &windBox,const glm::vec3 & direction,float speed);
        virtual ~WindParticleAffector();
        void Affect(float dt,Particle & particleToAffect);
    protected:
    private:
};

#endif // WINDPARTICLEAFFECTOR_H
