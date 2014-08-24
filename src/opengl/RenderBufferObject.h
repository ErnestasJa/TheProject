#ifndef RENDER_BUFFER_OBJECT_H
#define RENDER_BUFFER_OBJECT_H

#include "GLObject.h"

struct render_BufferObject: public gl_object
{
    static uint32_t current;

    render_BufferObject();
    virtual ~render_BufferObject();

    void init(uint32_t internal_format, uint32_t w, uint32_t h);
    void set();
    void unset();

    virtual GLO_TYPE get_type();

};

typedef std::shared_ptr<render_BufferObject> render_BufferObject_ptr;

#endif // RENDER_BUFFER_OBJECT_H
