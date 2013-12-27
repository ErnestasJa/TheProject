#include "precomp.h"
#include "buffer_object.h"

template <>
uint32_t index_buffer_object<uint8_t>::get_data_type()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t index_buffer_object<uint8_t>::get_component_count()
{
    return 1;
}

template <>
void index_buffer_object<uint8_t>::generate()
{
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
}

template <>
uint32_t index_buffer_object<uint32_t>::get_data_type()
{
    return GL_UNSIGNED_INT;
}

template <>
uint32_t index_buffer_object<uint32_t>::get_component_count()
{
    return 1;
}

template <>
void index_buffer_object<uint32_t>::generate()
{
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
}

template <>
uint32_t buffer_object<uint8_t>::get_data_type()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t buffer_object<uint8_t>::get_component_count()
{
    return 1;
}

template <>
void buffer_object<uint8_t>::generate()
{
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
}

template <>
uint32_t buffer_object<u8vec4>::get_data_type()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t buffer_object<u8vec4>::get_component_count()
{
    return 4;
}

template <>
void buffer_object<u8vec4>::generate()
{
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(u8vec4),&data[0],GL_STATIC_DRAW);
}

template <>
uint32_t buffer_object<uint32_t>::get_data_type()
{
    return GL_UNSIGNED_INT;
}

template <>
uint32_t buffer_object<uint32_t>::get_component_count()
{
    return 1;
}

template <>
void buffer_object<uint32_t>::generate()
{
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
}

template <>
uint32_t buffer_object<glm::vec4>::get_data_type()
{
    return GL_FLOAT;
}

template <>
uint32_t buffer_object<glm::vec4>::get_component_count()
{
    return 4;
}


template <>
void buffer_object<glm::vec4>::generate()
{
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec4), &data[0], GL_STATIC_DRAW);
}


template <>
uint32_t buffer_object<glm::vec3>::get_data_type()
{
    return GL_FLOAT;
}

template <>
uint32_t buffer_object<glm::vec3>::get_component_count()
{
    return 3;
}


template <>
void buffer_object<glm::vec3>::generate()
{
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);
}


template <>
uint32_t buffer_object<glm::vec2>::get_data_type()
{
    return GL_FLOAT;
}

template <>
uint32_t buffer_object<glm::vec2>::get_component_count()
{
    return 2;
}


template <>
void buffer_object<glm::vec2>::generate()
{
    glGenBuffers(1, &this->id);
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec2), &data[0], GL_STATIC_DRAW);
}
