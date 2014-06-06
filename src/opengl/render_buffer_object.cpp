#include "precomp.h"
#include "render_buffer_object.h"

uint32_t render_buffer_object::current = 0;

render_buffer_object::render_buffer_object()
{
    id=0;
}

render_buffer_object::~render_buffer_object()
{
    //dtor
}

void render_buffer_object::set()
{
    if(current!=id)
    {
        current = id;
        glBindRenderbuffer(GL_RENDERBUFFER,id);
    }
}

void render_buffer_object::unset()
{
    current = 0;
    glBindRenderbuffer(GL_RENDERBUFFER,0);
}

void render_buffer_object::init(uint32_t internal_format, uint32_t w, uint32_t h)
{
    glGenRenderbuffers(1, &id);

    set();
    glRenderbufferStorage(GL_RENDERBUFFER, internal_format, w, h);
}

GLO_TYPE render_buffer_object::get_type()
{
    return GLO_RENDERBUFFER;
}
