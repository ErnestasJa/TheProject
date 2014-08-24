#include "Precomp.h"
#include "RenderBufferObject.h"

uint32_t render_BufferObject::current = 0;

render_BufferObject::render_BufferObject()
{
    Id=0;
}

render_BufferObject::~render_BufferObject()
{
    //dtor
}

void render_BufferObject::set()
{
    if(current!=Id)
    {
        current = Id;
        glBindRenderbuffer(GL_RENDERBUFFER,Id);
    }
}

void render_BufferObject::unset()
{
    current = 0;
    glBindRenderbuffer(GL_RENDERBUFFER,0);
}

void render_BufferObject::init(uint32_t internal_format, uint32_t w, uint32_t h)
{
    glGenRenderbuffers(1, &Id);

    set();
    glRenderbufferStorage(GL_RENDERBUFFER, internal_format, w, h);
}

GLO_TYPE render_BufferObject::get_type()
{
    return GLO_RENDERBUFFER;
}
