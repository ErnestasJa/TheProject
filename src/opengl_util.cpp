#include "opengl_util.h"

#ifdef __MINGW32__
#include <GLXW/glxw_wgl.h>
#include <GL/wglext.h>
#else
#include <GLXW/glxw_glx.h>
#include <GL/glxext.h>
#endif

opengl_util::opengl_util()
{

}

opengl_util::~opengl_util()
{

}

bool opengl_util::load_extensions()
{
    if(glxwInit()!=0)
        return false;

    return true;
}

// TODO (serengeor#1#): log this
bool opengl_util::check_and_output_errors()
{
    uint32_t err = GL_NO_ERROR;

    bool ret=false;

    while((err=glGetError())!=GL_NO_ERROR)
    {
        std::cout<<"GL_ERROR: " << std::hex << err << std::endl;
        ret = true;
    }

    return ret;
}
