#ifndef RENDER_BUFFER_OBJECT_H
#define RENDER_BUFFER_OBJECT_H

#include "gl_object.h"

struct render_buffer_object: public gl_object
{
    static uint32_t current;

    render_buffer_object();
    virtual ~render_buffer_object();

    void init(uint32_t internal_format, uint32_t w, uint32_t h);
    void set();
    void unset();

    virtual GLO_TYPE get_type();

};

typedef std::shared_ptr<render_buffer_object> render_buffer_object_ptr;

#endif // RENDER_BUFFER_OBJECT_H
