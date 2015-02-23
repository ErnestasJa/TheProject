#ifndef WINDPARTICLEAFFECTOR_H
#define WINDPARTICLEAFFECTOR_H

#include "IParticleAffector.h"

#include "opengl/AABB.h"

class WindParticleAffector:public IParticleAffector
{
    public:
        WindParticleAffector(const AABB &windBox,const glm::vec3 & direction,float speed);
        virtual ~WindParticleAffector(){};
        void Affect(float dt,Particle & particleToAffect);
    protected:
    private:
        AABB _windBox;
        glm::vec3 _direction;
        float _speed;
};

#endif // WINDPARTICLEAFFECTOR_H
