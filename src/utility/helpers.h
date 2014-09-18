#pragma once
#include <sstream>
namespace helpers
{
#define loop(var,count) for(uint32_t (var) = 0; (var) < (count); (var)++)
#define loopi(count) for(uint32_t i = 0; i < (count); i++)
#define ROUNDING_ERROR 0.00001f

template <typename T> inline std::string to_str(const T& t)
{
    std::ostringstream os;    //magic..
    os<<t;
    return os.str();
}

template <typename T> inline std::wstring to_wstr(const T& t)
{
    std::wostringstream os;    //magic..
    os<<t;
    return os.str();
}

template <typename T>
inline T limit(T val, T min, T max)
{
    T ret;
    if(val<min)
        ret=min;
    else if(val>max)
        ret=max;
    else
        ret=val;

    return ret;
}

struct u8vec4{uint8_t v[4];};

inline uint32_t read(const std::string & file, char *& buf)
{
    printf("GET REAL DIR: %s\n",PHYSFS_getRealDir(file.c_str()));
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

inline const glm::vec4 color255(uint8_t r=255,uint8_t g=255,uint8_t b=255,uint8_t a=255)
{
    return glm::vec4((float)r/255,(float)g/255,(float)b/255,(float)a/255);
}

inline void invert(glm::vec3 & vec)
{
    if(vec.x)
        vec.x = 1.0f / vec.x;
    if(vec.y)
        vec.y = 1.0f / vec.y;
    if(vec.z)
        vec.z = 1.0f / vec.z;
}

template <class T>
bool equals(const T & v1, const T & v2);

template <>
inline bool equals(const float & v1, const float & v2)
{
    return (v1 + ROUNDING_ERROR >= v2) && (v1 - ROUNDING_ERROR <= v2);
}

template <>
inline bool equals(const glm::vec3 & v1, const glm::vec3 & v2)
{
    return equals(v1.x,v2.x) && equals(v1.y,v2.y) && equals(v1.z,v2.z);
}

template <>
inline bool equals(const glm::vec4 & v1, const glm::vec4 & v2)
{
    return equals(v1.x,v2.x) && equals(v1.y,v2.y) && equals(v1.z,v2.z) && equals(v1.w,v2.w);
}

template <>
inline bool equals(const glm::quat & v1, const glm::quat & v2)
{
    return equals(v1.x,v2.x) && equals(v1.y,v2.y) && equals(v1.z,v2.z) && equals(v1.w,v2.w);
}

template <>
inline bool equals(const glm::mat4 & v1, const glm::mat4 & v2)
{
    return equals(v1[0],v2[0]) && equals(v1[1],v2[1]) && equals(v1[2],v2[2]) && equals(v1[3],v2[3]);
}

template <>
inline bool equals(const glm::mat3 & v1, const glm::mat3 & v2)
{
    return equals(v1[0],v2[0]) && equals(v1[1],v2[1]) && equals(v1[2],v2[2]);
}

inline float coslerp(float y1,float y2,float mu)
{
   float mu2;

   mu2 = (1-glm::cos(mu*glm::pi<float>()))/2;
   return(y1*(1-mu2)+y2*mu2);
}

}

#include <assert.h>
///TO IMPLEMENT!
inline const char* getErr(uint32_t err)
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
inline void CheckOpenGLError(const char* stmt, const char* fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        printf("OpenGL error %s:\n%s:%i\n%s\n", getErr(err), fname, line, stmt);
        abort();
    }
}

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif
