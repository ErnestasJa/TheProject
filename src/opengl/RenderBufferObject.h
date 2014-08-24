#ifndef RENDER_BUFFER_OBJECT_H
#define RENDER_BUFFER_OBJECT_H

#include "GLObject.h"

struct RenderBufferObject: public GLObject
{
    static uint32_t current;

    RenderBufferObject();
    virtual ~RenderBufferObject();

    void Init(uint32_t internal_format, uint32_t w, uint32_t h);
    void Set();
    void unset();

    virtual GLO_TYPE GetType();

};

typedef std::shared_ptr<RenderBufferObject> render_BufferObject_ptr;

#endif // RENDER_BUFFER_OBJECT_H
