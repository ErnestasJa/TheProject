#include "opengl_util.h"
#include "logger.h"


#ifdef __MINGW32__
#include <GLXW/glxw_wgl.h>
#include <GL/wglext.h>
#else
#include <GLXW/glxw_glx.h>
#include <GL/glxext.h>
#endif

opengl_util::opengl_util(logger * l)
{
    m_logger = l;
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
        m_logger->log(LOG_ERROR,"GL_ERROR: %s",gl_error_to_string(err).c_str());

        ret = true;
    }

    return ret;
}

std::string opengl_util::gl_error_to_string(uint32_t error)
{

    switch(error)
    {
        case 0:
            return "GL_NO_ERROR";
        case 0x0500:
            return "GL_INVALID_ENUM";
        case 0x0501:
            return "GL_INVALID_VALUE";
        case 0x0502:
            return "GL_INVALID_OPERATION";
        case 0x0503:
            return "GL_STACK_OVERFLOW";
        case 0x0504:
            return "GL_STACK_UNDERFLOW";
        case 0x0505:
            return "GL_OUT_OF_MEMORY";
        case 0x0506:
            return "GL_INVALID_FRAMEBUFFER_OPERATION";
        default:
            return "BAD ERROR CODE";

    }
}

std::string opengl_util::gl_fbo_error_to_string(uint32_t error)
{

    switch(error)
    {
    case 0x8CD5:
        return "GL_FRAMEBUFFER_COMPLETE";
    case 0x8CD6:
        return "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
    case 0x8CD7:
        return "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
    case 0x8CDB:
        return "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
    case 0x8CDC:
        return "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
    case 0x8CDD:
        return "GL_FRAMEBUFFER_UNSUPPORTED";
    default:
        return "BAD ERROR CODE";

    }
}
