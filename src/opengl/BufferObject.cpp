#include "Precomp.h"
#include "BufferObject.h"

/// index buffer objects
template <>
uint32_t IndexBufferObject<uint8_t>::get_data_type()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t IndexBufferObject<uint8_t>::get_component_count()
{
    return 1;
}

template <>
void IndexBufferObject<uint8_t>::init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void IndexBufferObject<uint8_t>::upload()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

template <>
uint32_t IndexBufferObject<uint32_t>::get_data_type()
{
    return GL_UNSIGNED_INT;
}

template <>
uint32_t IndexBufferObject<uint32_t>::get_component_count()
{
    return 1;
}

template <>
void IndexBufferObject<uint32_t>::init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void IndexBufferObject<uint32_t>::upload()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

///buffer objects
template <>
uint32_t BufferObject<uint8_t>::get_data_type()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t BufferObject<uint8_t>::get_component_count()
{
    return 1;
}

template <>
void BufferObject<uint8_t>::init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<uint8_t>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(uint8_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<helpers::u8vec4>::get_data_type()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t BufferObject<helpers::u8vec4>::get_component_count()
{
    return 4;
}

template <>
void BufferObject<helpers::u8vec4>::init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<helpers::u8vec4>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(helpers::u8vec4),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<uint32_t>::get_data_type()
{
    return GL_UNSIGNED_INT;
}

template <>
uint32_t BufferObject<uint32_t>::get_component_count()
{
    return 1;
}

template <>
void BufferObject<uint32_t>::init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<uint32_t>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<glm::vec4>::get_data_type()
{
    return GL_FLOAT;
}

template <>
uint32_t BufferObject<glm::vec4>::get_component_count()
{
    return 4;
}

template <>
void BufferObject<glm::vec4>::init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<glm::vec4>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec4), &data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<glm::vec3>::get_data_type()
{
    return GL_FLOAT;
}

template <>
uint32_t BufferObject<glm::vec3>::get_component_count()
{
    return 3;
}

template <>
void BufferObject<glm::vec3>::init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<glm::vec3>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<glm::vec2>::get_data_type()
{
    return GL_FLOAT;
}

template <>
uint32_t BufferObject<glm::vec2>::get_component_count()
{
    return 2;
}

template <>
void BufferObject<glm::vec2>::init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<glm::vec2>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec2), &data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}
