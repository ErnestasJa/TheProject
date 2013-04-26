#ifndef __UTIL_H__
#define __UTIL_H__

#ifdef NULL
#undef NULL
#endif
#define NULL 0

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

#ifdef swap
#undef swap
#endif
template<class T>
static inline void swap(T &a, T &b)
{
    T t = a;
    a = b;
    b = t;
}
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif
template<class T>
static inline T max(T a, T b)
{
    return a > b ? a : b;
}
template<class T>
static inline T min(T a, T b)
{
    return a < b ? a : b;
}

template<class T> static inline T radians(T x) { return (x*M_PI)/180; }
template<class T> static inline T degrees(T x) { return (x*180)/M_PI; }

template<class T>
static inline T clamp(T val, T minval, T maxval)
{
    return max(minval, min(val, maxval));
}

inline bool islittleendian() { static const int val = 1; return *(const uchar *)&val != 0; }
inline ushort endianswap16(ushort n) { return (n<<8) | (n>>8); }
inline uint endianswap32(uint n) { return (n<<24) | (n>>24) | ((n>>8)&0xFF00) | ((n<<8)&0xFF0000); }
template<class T> inline T endianswap(T n) { union { T t; uint i; } conv; conv.t = n; conv.i = endianswap32(conv.i); return conv.t; }
template<> inline ushort endianswap<ushort>(ushort n) { return endianswap16(n); }
template<> inline short endianswap<short>(short n) { return endianswap16(n); }
template<> inline uint endianswap<uint>(uint n) { return endianswap32(n); }
template<> inline int endianswap<int>(int n) { return endianswap32(n); }
template<class T> inline void endianswap(T *buf, int len) { for(T *end = &buf[len]; buf < end; buf++) *buf = endianswap(*buf); }
template<class T> inline T lilswap(T n) { return islittleendian() ? n : endianswap(n); }
template<class T> inline void lilswap(T *buf, int len) { if(!islittleendian()) endianswap(buf, len); }
template<class T> inline T bigswap(T n) { return islittleendian() ? endianswap(n) : n; }
template<class T> inline void bigswap(T *buf, int len) { if(islittleendian()) endianswap(buf, len); }

template<class T> T getval(FILE *f) { T n; return fread(&n, 1, sizeof(n), f) == sizeof(n) ? n : 0; }
template<class T> T getlil(FILE *f) { return lilswap(getval<T>(f)); }
template<class T> T getbig(FILE *f) { return bigswap(getval<T>(f)); }

inline void invert_glm(glm::mat4 & o)
{
    glm::mat3x3 invrot(glm::vec3(o[0].x, o[1].x, o[2].x), glm::vec3(o[0].y, o[1].y, o[2].y), glm::vec3(o[0].z, o[1].z, o[2].z));

    invrot[0] /= glm::length2(invrot[0]);
    invrot[1] /= glm::length2(invrot[1]);
    invrot[2] /= glm::length2(invrot[2]);
    glm::vec3 trans(o[0].w, o[1].w, o[2].w);

    o[0] = glm::vec4(invrot[0], -glm::dot(invrot[0],trans));
    o[1] = glm::vec4(invrot[1], -glm::dot(invrot[1],trans));
    o[2] = glm::vec4(invrot[2], -glm::dot(invrot[2],trans));

}

inline void convertquat(glm::mat4 & mat, const glm::quat & q)
{

    float x = q.x, y = q.y, z = q.z, w = q.w,
          tx = 2*x, ty = 2*y, tz = 2*z,
          txx = tx*x, tyy = ty*y, tzz = tz*z,
          txy = tx*y, txz = tx*z, tyz = ty*z,
          twx = w*tx, twy = w*ty, twz = w*tz;
    mat[0] = glm::vec4(1.0f - (tyy + tzz), txy - twz, txz + twy,0);
    mat[1] = glm::vec4(txy + twz, 1.0f - (txx + tzz), tyz - twx,0);
    mat[2] = glm::vec4(txz - twy, tyz + twx, 1.0f - (txx + tyy),0);
    mat[3] = glm::vec4(0, 0, 0,1);

}

inline void scale(glm::vec4 & v, const glm::vec3& s)
{
    v.x*=s.x;
    v.y*=s.y;
    v.z*=s.z;
}

inline void makeJointMatrix(glm::mat4 & mat, const glm::quat & rot, const glm::vec3& pos, const glm::vec3& s)
{
    convertquat(mat,rot);

    scale(mat[0], s);
    scale(mat[1], s);
    scale(mat[2], s);

    mat[0].w= pos.x;
    mat[1].w= pos.y;
    mat[2].w= pos.z;
}

#endif

