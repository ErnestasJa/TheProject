#include "precomp.h"
#include "buffer_object.h"

/// index buffer objects
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
void index_buffer_object<uint8_t>::init()
{
    glGenBuffers(1, &this->id);
}

template <>
void index_buffer_object<uint8_t>::upload()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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
void index_buffer_object<uint32_t>::init()
{
    glGenBuffers(1, &this->id);
}

template <>
void index_buffer_object<uint32_t>::upload()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

///buffer objects
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
void buffer_object<uint8_t>::init()
{
    glGenBuffers(1, &this->id);
}

template <>
void buffer_object<uint8_t>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(uint8_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t buffer_object<helpers::u8vec4>::get_data_type()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t buffer_object<helpers::u8vec4>::get_component_count()
{
    return 4;
}

template <>
void buffer_object<helpers::u8vec4>::init()
{
    glGenBuffers(1, &this->id);
}

template <>
void buffer_object<helpers::u8vec4>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(helpers::u8vec4),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
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
void buffer_object<uint32_t>::init()
{
    glGenBuffers(1, &this->id);
}

template <>
void buffer_object<uint32_t>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
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
void buffer_object<glm::vec4>::init()
{
    glGenBuffers(1, &this->id);
}

template <>
void buffer_object<glm::vec4>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec4), &data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
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
void buffer_object<glm::vec3>::init()
{
    glGenBuffers(1, &this->id);
}

template <>
void buffer_object<glm::vec3>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
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
void buffer_object<glm::vec2>::init()
{
    glGenBuffers(1, &this->id);
}

template <>
void buffer_object<glm::vec2>::upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec2), &data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}
