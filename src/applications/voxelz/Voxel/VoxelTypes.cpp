#include "Precomp.h"
#include "VoxelTypes.h"
#include "opengl/BufferObject.h"

template <>
uint32_t IndexBufferObject<uint16_t>::GetDataType()
{
    return GL_UNSIGNED_SHORT;
}

template <>
uint32_t IndexBufferObject<uint16_t>::GetComponentCount()
{
    return 1;
}

template <>
void IndexBufferObject<uint16_t>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void IndexBufferObject<uint16_t>::Upload()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(uint16_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



template <>
uint32_t BufferObject<u8vec3>::GetDataType()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t BufferObject<u8vec3>::GetComponentCount()
{
    return 3;
}

template <>
void BufferObject<u8vec3>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<u8vec3>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(u8vec3),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}



template <>
uint32_t BufferObject<u8vec4>::GetDataType()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t BufferObject<u8vec4>::GetComponentCount()
{
    return 4;
}

template <>
void BufferObject<u8vec4>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<u8vec4>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(u8vec4),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}



template <>
uint32_t BufferObject<s8vec3>::GetDataType()
{
    return GL_BYTE;
}

template <>
uint32_t BufferObject<s8vec3>::GetComponentCount()
{
    return 3;
}

template <>
void BufferObject<s8vec3>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<s8vec3>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(s8vec3),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}



template <>
uint32_t BufferObject<s16vec3>::GetDataType()
{
    return GL_BYTE;
}

template <>
uint32_t BufferObject<s16vec3>::GetComponentCount()
{
    return 3;
}

template <>
void BufferObject<s16vec3>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<s16vec3>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(s16vec3),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}
