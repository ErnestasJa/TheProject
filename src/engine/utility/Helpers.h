#ifndef _HELPERS_H_INCLUDED_
#define _HELPERS_H_INCLUDED_

#include <sstream>

#define loop(var,count) for(uint32_t (var) = 0; (var) < (count); (var)++)
#define loopi(count) for(int32_t i = 0; i < (count); i++)
#define loopr(var,start,count) for(uint32_t (var) = (start); (var) < (count); (var)++)
#define loopxyz(cx,cy,cz) loop(z,(cz)) loop(y,(cy)) loop(x,(cx))
#define comp(vec) (vec).x, (vec).y, (vec).z
#define ROUNDING_ERROR 0.00001f

namespace helpers
{
uint32_t read(const std::string & file, char *& buf);

struct u8vec4
{
    uint8_t v[4];
};

template <typename T> inline std::string to_str(const T& t);
template <typename T> inline std::wstring to_wstr(const T& t);
template <typename T> inline T limit(T val, T min, T max);
const inline glm::vec4 color255(uint8_t r,uint8_t g,uint8_t b,uint8_t a);
inline void invert(glm::vec3 & vec);
template <class T> inline bool equals(const T & v1, const T & v2);
inline float coslerp(float y1,float y2,float mu);
inline glm::vec3 hsv2rgb(float h,float s,float v);

inline glm::vec3 rgb2hsv(float r, float g, float b);

inline int wtoi(const wchar_t *str)
{
  return (int)wcstol(str, 0, 10);
}

#include "Helpers.inl"
}

template <class T>
inline std::shared_ptr<T> share(T* t)
{
    return std::shared_ptr<T>(t);
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

#endif // _HELPERS_H_INCLUDED_
