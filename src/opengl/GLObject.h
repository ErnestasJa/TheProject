#ifndef GL_OBJECT_H
#define GL_OBJECT_H

enum GLO_TYPE
{
    GLO_TEXTURE=0,
    GLO_FRAMEBUFFER,
    GLO_RENDERBUFFER,
    GLO_VERTEX_ARRAY_OBJECT,
    GLO_BUFFER_OBJECT,
    GLO_SHADER_PROGRAM
};

struct gl_object
{
    uint32_t Id;

    gl_object();
    virtual ~gl_object();

    virtual GLO_TYPE get_type()=0;

};

typedef std::shared_ptr<gl_object> gl_object_ptr;

#endif // GL_OBJECT_H
