#include "Precomp.h"

#include "ParticleSystem.h"
#include "ParticleEmitter.h"
#include "opengl/OpenGLUtil.h"
#include "opengl/AABB.h"

ParticleSystem::ParticleSystem()
{
    _lastUsedParticle=0;
    _rotate=0;

    _geometry=new BufferObject<glm::vec3>();
    _geometry->Init();
    _geometry->data.push_back(glm::vec3(0));
    _geometry->Upload();

    _pos=new BufferObject<glm::vec3>();
    _pos->Init();
    glBindBuffer(GL_ARRAY_BUFFER,_pos->Id);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES*sizeof(glm::vec3),0,GL_STREAM_DRAW);

    _col=new BufferObject<u8vec4>();
    _col->Init();
    glBindBuffer(GL_ARRAY_BUFFER,_col->Id);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES*sizeof(u8vec4),0,GL_STREAM_DRAW);

    glGenVertexArrays(1,&_VAO);
    glBindVertexArray(_VAO);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _geometry->Id);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, _pos->Id);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,0,(void*)0);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, _col->Id);
    glVertexAttribPointer(2,4,GL_UNSIGNED_BYTE,GL_TRUE,0,(void*)0);

    glBindVertexArray(0);

    _pos->data.resize(MAX_PARTICLES);
    _col->data.resize(MAX_PARTICLES);
}

ParticleSystem::~ParticleSystem()
{
    delete _geometry;
    delete _pos;
    delete _col;
    glDeleteBuffers(1,&_VAO);
}

uint32_t ParticleSystem::FindUnused()
{
    if(_lastUsedParticle>=MAX_PARTICLES/2)
    {
        loopr(i,_lastUsedParticle,MAX_PARTICLES)
        {
            if(_particlesContainer[i].life<=0.f)
            {
                _lastUsedParticle=i;
                return i;
            }
        }
    }
    else
    {
        loop(i,_lastUsedParticle)
        {
            if(_particlesContainer[i].life<=0.f)
            {
                _lastUsedParticle=i;
                return i;
            }
        }
    }
    return 0;
}

void ParticleSystem::AddEmitter(ParticleEmitter* emitter)
{
    _emitters.push_back(emitter);
}

void ParticleSystem::Update(float dt)
{
    for(auto emitter:_emitters)
    {
        uint32_t newparticles = (uint32_t)(dt*emitter->_maxParticles);
        uint32_t maxnewparticles=(uint32_t)(0.0167f*emitter->_maxParticles);

        if(newparticles>maxnewparticles)
        {
            newparticles=maxnewparticles;
        }

        loop(i,newparticles)
        {
            uint32_t emitterParticleIndex=emitter->FindUnused();
            if(emitterParticleIndex==-1) break;

            if(emitter->_particleCount>emitter->_maxParticles)break;
            uint32_t particleIndex=FindUnused();
            Particle& currentParticle=_particlesContainer[particleIndex];

            BuildParticle(currentParticle,emitter);

            emitter->_particleContainer[emitterParticleIndex]=currentParticle;
        }
    }

    _particleCount=0;
    for(auto emitter:_emitters)
    {
        emitter->Update(dt,_particleCount,_pos,_col);
    }
}

void ParticleSystem::BuildParticle(Particle& p,ParticleEmitter* e)
{
    p.life=e->_particleLife;
    p.pos=e->_pos;
    float spread=e->_spread;
    glm::vec3 mainDir=e->_direction;
    glm::vec3 randomDir((rand()%2000-1000.f)/1000.f,(rand()%2000-1000.f)/1000.f,(rand()%2000-1000.f)/1000.f);
    p.speed=mainDir+randomDir*spread;
    p.col=u8vec4(rand()%256,rand()%256,rand()%256,255);
    p.size = e->_particleSize;
}

void ParticleSystem::Render()
{
//    printf("Drawing %u particles\n",_particleCount);

    glBindBuffer(GL_ARRAY_BUFFER, _pos->Id);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(glm::vec3), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf.
    glBufferSubData(GL_ARRAY_BUFFER, 0, _particleCount * sizeof(glm::vec3), glm::value_ptr(_pos->data[0]));

    glBindBuffer(GL_ARRAY_BUFFER, _col->Id);
    glBufferData(GL_ARRAY_BUFFER, MAX_PARTICLES * sizeof(u8vec4), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf.
    glBufferSubData(GL_ARRAY_BUFFER, 0, _particleCount * sizeof(u8vec4), glm::value_ptr(_col->data[0]));

    glBindVertexArray(_VAO);

    glVertexAttribDivisor(0,0);
    glVertexAttribDivisor(1,1);
    glVertexAttribDivisor(2,1);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArraysInstanced(GL_POINTS,0,1,_particleCount);
    glDisable(GL_BLEND);

    glBindVertexArray(0);
}
