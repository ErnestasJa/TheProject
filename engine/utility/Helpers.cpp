#include "Precomp.h"
#include "Helpers.h"

namespace helpers
{
uint32_t read(const std::string & file, char *& buf)
{
    PHYSFS_file* f = PHYSFS_openRead(file.c_str());

    if(!f)
    {
        printf("PHYSFS: Opening (%s) to read failed.\n",file.c_str());
        return 0;
    }

    uint32_t len = PHYSFS_fileLength(f), ret = 0;

    buf = new char[len+1];
    buf[len] = 0;

    ret = PHYSFS_read(f, buf, len, 1)*len;


    PHYSFS_close(f);

    return ret;
}

#include <assert.h>
const char* getErr(uint32_t err)
{
    switch(err)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
        break;
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
        break;
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
        break;
    case GL_STACK_OVERFLOW:
        return "GL_STACK_OVERFLOW";
        break;
    case GL_STACK_UNDERFLOW:
        return "GL_STACK_UNDERFLOW";
        break;
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
        break;
    default:
        return "UNKNOWN_ERROR";
        break;
    }
}

void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("OpenGL error %s:\n%s:%i\n%s\n", getErr(err), fname, line, stmt);
        abort();
    }
}

}
