#pragma once
#include <sstream>
namespace helpers
{
#define loop(var,count) for(uint32_t (var) = 0; (var) < (count); (var)++)
#define loopr(var,start,count) for(uint32_t (var) = (start); (var) < (count); (var)++)
#define loopi(count) for(uint32_t i = 0; i < (count); i++)
#define loopxyz(cx,cy,cz) loop(z,(cz)) loop(y,(cy)) loop(x,(cx))
#define comp(vec) (vec).x, (vec).y, (vec).z
#define ROUNDING_ERROR 0.00001f

struct u8vec4
{
    uint8_t v[4];
};

uint32_t read(const std::string & file, char *& buf);

template <typename T> inline std::string to_str(const T& t);
template <typename T> inline std::wstring to_wstr(const T& t);
template <typename T> inline T limit(T val, T min, T max);
inline const glm::vec4 color255(uint8_t r=255,uint8_t g=255,uint8_t b=255,uint8_t a=255);
inline void invert(glm::vec3 & vec);
template <class T> bool equals(const T & v1, const T & v2);
inline float coslerp(float y1,float y2,float mu);
inline glm::vec3 hsv2rgb(float h,float s,float v);

#include "helpers.inl"
}

///TO IMPLEMENT!
const char* getErr(uint32_t err);
void CheckOpenGLError(const char* stmt, const char* fname, int line);

#ifdef _DEBUG
#define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
#define GL_CHECK(stmt) stmt
#endif
