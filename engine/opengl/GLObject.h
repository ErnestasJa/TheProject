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

struct GLObject
{
    uint32_t Id;

    GLObject();
    virtual ~GLObject();

    virtual GLO_TYPE GetType()=0;

};

typedef std::shared_ptr<GLObject> gl_object_ptr;

#endif // GL_OBJECT_H
