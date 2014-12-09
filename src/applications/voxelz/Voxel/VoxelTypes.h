#ifndef VOXELTYPES_H_INCLUDED
#define VOXELTYPES_H_INCLUDED

typedef glm::detail::tvec4<uint8_t> u8vec4;
typedef glm::detail::tvec4<uint16_t> u16vec4;

typedef glm::detail::tvec3<int8_t> s8vec3;
typedef glm::detail::tvec3<int16_t> s16vec3;
typedef glm::detail::tvec3<uint8_t> u8vec3;
typedef glm::detail::tvec3<uint16_t> u16vec3;

typedef uint32_t intRGBA;

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

#endif // VOXELTYPES_H_INCLUDED
