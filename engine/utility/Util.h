#pragma once

//Move these defines to some other file
#ifndef M_PI
#define M_PI 3.14159265359
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL 0

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

template<class T> static inline T radians(T x) { return (x*M_PI)/180; }
template<class T> static inline T degrees(T x) { return (x*180)/M_PI; }

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

void print_glm(const glm::mat4 & m);
void invert(glm::mat3x4 & dest, const glm::mat3x4 & o);
void convertquat(glm::mat3x4 & mat, const glm::quat & q);
void scale(glm::vec4 & v, const glm::vec3& s);
void makeJointMatrix(glm::mat3x4 & mat, const glm::quat & rot, const glm::vec3& pos, const glm::vec3& s);
glm::mat3x4 mul(const glm::mat3x4 & m1, const glm::mat3x4 & m2);


#ifdef swap
#undef swap
#endif
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

template<class T>
static inline T clamp(T val, T minval, T maxval)
{
    return max(minval, min(val, maxval));
}

template<class T>
static inline void swap(T &a, T &b)
{
    T t = a;
    a = b;
    b = t;
}
