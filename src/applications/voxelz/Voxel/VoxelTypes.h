#ifndef VOXELTYPES_H_INCLUDED
#define VOXELTYPES_H_INCLUDED

#include <boost/unordered_map.hpp>
#include <boost/functional/hash.hpp>
#include <utility/vector.h>
#include "SimplexNoise.h"

typedef glm::detail::tvec4<uint8_t> u8vec4;
typedef glm::detail::tvec4<uint16_t> u16vec4;

typedef glm::detail::tvec3<int8_t> s8vec3;
typedef glm::detail::tvec3<int16_t> s16vec3;
typedef glm::detail::tvec3<uint8_t> u8vec3;
typedef glm::detail::tvec3<uint16_t> u16vec3;

typedef uint32_t intRGBA;

template<typename T>
inline static const char* GLMVec3ToStr(const T & in)
{
    glm::vec3 vec=(glm::vec3)in;
    char buf[256];
    sprintf(buf,"vec3(%f %f %f)",vec.x,vec.y,vec.z);
    return std::string(buf).c_str();
}

inline static uint32_t VecRGBToIntRGB(const u8vec3& col)
{
    uint8_t r = col.x & 0xFF;
    uint8_t g = col.y & 0xFF;
    uint8_t b = col.z & 0xFF;

    uint32_t rgba = (r << 24) + (g << 16) + (b << 8);

    return rgba;
}

inline static uint32_t VecRGBAToIntRGBA(const u8vec4& col)
{
    uint8_t r = col.x & 0xFF;
    uint8_t g = col.y & 0xFF;
    uint8_t b = col.z & 0xFF;
    uint8_t a = col.w & 0xFF;

    uint32_t rgba = (r << 24) + (g << 16) + (b << 8) + (a);

    return rgba;
}

inline static u8vec3 IntRGBToVecRGB(const intRGBA &col)
{
    return u8vec3(col>>24&0xFF,col>>16&0xFF,col>>8&0xFF);
}

inline static u8vec4 IntRGBAToVecRGBA(const intRGBA &col)
{
    return u8vec4(col>>24&0xFF,col>>16&0xFF,col>>8&0xFF,col&0xFF);
}

struct ivec3hash : std::unary_function<glm::ivec3, std::size_t>
{
    std::size_t operator()(glm::ivec3 const& v) const
    {
        std::size_t seed = 0;
        boost::hash_combine(seed, v.x);
        std::size_t a=seed;
        boost::hash_combine(seed, v.y);
        std::size_t b=seed;
        boost::hash_combine(seed, v.z);
        std::size_t c=seed;

        return seed;
    }
};

struct ivec3equal
{
    bool operator() (glm::ivec3 const& a, glm::ivec3 const& b) const
    {
        return a.x==b.x && a.y==b.y && a.z==b.z;
    }
};

struct GLMVec3Compare
{
    bool operator() (const glm::vec3 &a,const glm::vec3 &b) const
    {
        return
            (a.x<b.x || !(a.x==b.x)) ||
            (a.x==b.x && a.y<b.y && !(a.y==b.y)) ||
            (a.x==b.x && a.y==b.y && a.z<b.z && !(a.z==b.z));
    }
};

template <typename T>
inline void appendVectors(vector<T> &a,vector<T> &b)
{
    a.reserve(a.size()+b.size());
    a.insert(a.end(), b.begin(), b.end());
}

template <typename T>
inline void freeVector(vector<T> &a)
{
    vector<T>().swap(a);
}

template <typename T,typename U,typename V>
struct VoxMeshData
{
    vector<T> positions;
    vector<U> colors;
    vector<V> indices;

    bool empty;

    void Free()
    {
        positions.clear();
        colors.clear();
        indices.clear();
    }
};

#endif // VOXELTYPES_H_INCLUDED
