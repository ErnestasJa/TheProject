#include "Precomp.h"

#include "ParticleSystem.h"
#include "opengl/OpenGLUtil.h"

ParticleSystem::ParticleSystem()
{
    _lastUsedParticle=0;

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

void ParticleSystem::Update(float dt)
{
    uint32_t newparticles = (uint32_t)(dt*10000.f);
    uint32_t maxnewparticles=(uint32_t)(0.016f*10000.f);
    if(newparticles>maxnewparticles)
    {
        newparticles=maxnewparticles;
    }

    loop(i,newparticles)
    {
        uint32_t particleIndex=FindUnused();
        Particle& currentParticle=_particlesContainer[particleIndex];

        currentParticle.life=5.0f;
        currentParticle.pos=glm::vec3(0.f);

        float spread=3.f;
        glm::vec3 mainDir=glm::vec3(0.f,10.f,0.f);

        glm::vec3 randomDir((rand()%2000-1000.f)/1000.f,(rand()%2000-1000.f)/1000.f,(rand()%2000-1000.f)/1000.f);

        currentParticle.speed=mainDir+randomDir*spread;

        currentParticle.col=u8vec4(rand()%256,rand()%256,rand()%256,(rand()%256)/2.f);

        currentParticle.size =(rand()%1000)/2000.0f + 0.1f;
    }

    _particleCount=0;
    loop(i,MAX_PARTICLES)
    {
        Particle &p=_particlesContainer[i];

        if(p.life>0.f)
        {
            p.life-=dt;
            if(p.life>0.f)
            {
                p.speed+=glm::vec3(0.f,-9.81f,0.f)*dt*0.5f;
                p.pos+=p.speed * dt;

                _pos->data[_particleCount]=p.pos;
                _col->data[_particleCount]=p.col;
            }
            _particleCount++;
        }
    }
}

void ParticleSystem::Render(OpenGLUtil *util)
{
    //printf("Drawing %u particles\n",_particleCount);

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
