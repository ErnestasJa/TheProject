#include "Precomp.h"

#include "OpenGLUtil.h"
#include "utility/Logger.h"


#ifdef __MINGW32__
#include <GLXW/glxw_wgl.h>
#include <GL/wglext.h>
#else
#include <GLXW/glxw_glx.h>
#include "GL/glxext.h"
#endif

#include <assert.h>

OpenGLUtil::OpenGLUtil(Logger * l)
{
    _logger = l;
}

OpenGLUtil::~OpenGLUtil()
{

}

bool OpenGLUtil::load_extensions()
{
    if(glxwInit()!=0)
        return false;

    return true;
}

// TODO (serengeor#1#): log this
bool OpenGLUtil::check_and_output_errors()
{
    uint32_t err = GL_NO_ERROR;

    bool ret=false;

    while((err=glGetError())!=GL_NO_ERROR)
    {
        _logger->log(LOG_ERROR,"GL_ERROR: %s",gl_error_to_string(err).c_str());

        ///
        //assert(err==GL_NO_ERROR);

        ret = true;
    }

    return ret;
}

std::string OpenGLUtil::gl_error_to_string(uint32_t error)
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

std::string OpenGLUtil::gl_fbo_error_to_string(uint32_t error)
{
    std::string r;
    switch(error)
    {
    case 0x8CD5:
        r = "GL_FRAMEBUFFER_COMPLETE";
        break;
    case 0x8CD6:
        r = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
        break;
    case 0x8CD7:
        r = "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT";
        break;
    case 0x8CDB:
        r = "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER";
        break;
    case 0x8CDC:
        r = "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER";
        break;
    case 0x8CDD:
        r = "GL_FRAMEBUFFER_UNSUPPORTED";
        break;
    case 0x8D56:
        r = "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE";
        break;
    case 0x8DA8:
        r = "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS";
        break;
    case 0x8219:
        r = "GL_FRAMEBUFFER_UNDEFINED";
        break;
    default:
        r = "GL_BAD_ERROR_CODE: " + error; //fix this
        break;
    }
    r+=__FILE__;
    r+=__LINE__;
    return r;
}
