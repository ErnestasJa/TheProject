#include "Precomp.h"
#include "RenderBufferObject.h"

uint32_t RenderBufferObject::current = 0;

RenderBufferObject::RenderBufferObject()
{
    Id=0;
}

RenderBufferObject::~RenderBufferObject()
{
    //dtor
}

void RenderBufferObject::Set()
{
    if(current!=Id)
    {
        current = Id;
        glBindRenderbuffer(GL_RENDERBUFFER,Id);
    }
}

void RenderBufferObject::unset()
{
    current = 0;
    glBindRenderbuffer(GL_RENDERBUFFER,0);
}

void RenderBufferObject::Init(uint32_t internal_format, uint32_t w, uint32_t h)
{
    glGenRenderbuffers(1, &Id);

    Set();
    glRenderbufferStorage(GL_RENDERBUFFER, internal_format, w, h);
}

GLO_TYPE RenderBufferObject::GetType()
{
    return GLO_RENDERBUFFER;
}
