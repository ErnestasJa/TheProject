#include "Precomp.h"
#include "BufferObject.h"

/// index buffer objects
template <>
uint32_t IndexBufferObject<uint8_t>::GetDataType()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t IndexBufferObject<uint8_t>::GetComponentCount()
{
    return 1;
}

template <>
void IndexBufferObject<uint8_t>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void IndexBufferObject<uint8_t>::Upload()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

template <>
uint32_t IndexBufferObject<uint32_t>::GetDataType()
{
    return GL_UNSIGNED_INT;
}

template <>
uint32_t IndexBufferObject<uint32_t>::GetComponentCount()
{
    return 1;
}

template <>
void IndexBufferObject<uint32_t>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void IndexBufferObject<uint32_t>::Upload()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

///buffer objects
template <>
uint32_t BufferObject<uint8_t>::GetDataType()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t BufferObject<uint8_t>::GetComponentCount()
{
    return 1;
}

template <>
void BufferObject<uint8_t>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<uint8_t>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(uint8_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<helpers::u8vec4>::GetDataType()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t BufferObject<helpers::u8vec4>::GetComponentCount()
{
    return 4;
}

template <>
void BufferObject<helpers::u8vec4>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<helpers::u8vec4>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(helpers::u8vec4),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
void BufferObject<helpers::u8vec4>::UploadSubData(vector<helpers::u8vec4> subdata,uint32_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(helpers::u8vec4), subdata.size()*sizeof(helpers::u8vec4), &subdata[0]);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}


template <>
uint32_t BufferObject<glm::detail::tvec4<uint8_t> >::GetDataType()
{
    return GL_UNSIGNED_BYTE;
}

template <>
uint32_t BufferObject<glm::detail::tvec4<uint8_t> >::GetComponentCount()
{
    return 4;
}

template <>
void BufferObject<glm::detail::tvec4<uint8_t> >::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<glm::detail::tvec4<uint8_t> >::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::detail::tvec4<uint8_t> ),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
void BufferObject<glm::detail::tvec4<uint8_t> >::UploadSubData(vector<glm::detail::tvec4<uint8_t> > subdata,uint32_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(glm::detail::tvec4<uint8_t> ), subdata.size()*sizeof(glm::detail::tvec4<uint8_t> ), &subdata[0]);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

///

template <>
uint32_t BufferObject<uint32_t>::GetDataType()
{
    return GL_UNSIGNED_INT;
}

template <>
uint32_t BufferObject<uint32_t>::GetComponentCount()
{
    return 1;
}

template <>
void BufferObject<uint32_t>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<uint32_t>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(uint32_t),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<float>::GetDataType()
{
    return GL_FLOAT;
}

template <>
uint32_t BufferObject<float>::GetComponentCount()
{
    return 1;
}

template <>
void BufferObject<float>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<float>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(float),&data[0],GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
void BufferObject<float>::UploadSubData(vector<float> subdata,uint32_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(float), subdata.size()*sizeof(float), &subdata[0]);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<glm::vec4>::GetDataType()
{
    return GL_FLOAT;
}

template <>
uint32_t BufferObject<glm::vec4>::GetComponentCount()
{
    return 4;
}

template <>
void BufferObject<glm::vec4>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<glm::vec4>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec4), &data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
void BufferObject<glm::vec4>::UploadSubData(vector<glm::vec4> subdata,uint32_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(glm::vec4), subdata.size()*sizeof(glm::vec4), &subdata[0]);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<glm::vec3>::GetDataType()
{
    return GL_FLOAT;
}

template <>
uint32_t BufferObject<glm::vec3>::GetComponentCount()
{
    return 3;
}

template <>
void BufferObject<glm::vec3>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<glm::vec3>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec3), &data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
void BufferObject<glm::vec3>::UploadSubData(vector<glm::vec3> subdata,uint32_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(glm::vec3), subdata.size()*sizeof(glm::vec3), &subdata[0]);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
uint32_t BufferObject<glm::vec2>::GetDataType()
{
    return GL_FLOAT;
}

template <>
uint32_t BufferObject<glm::vec2>::GetComponentCount()
{
    return 2;
}

template <>
void BufferObject<glm::vec2>::Init()
{
    glGenBuffers(1, &this->Id);
}

template <>
void BufferObject<glm::vec2>::Upload()
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferData(GL_ARRAY_BUFFER, data.size()*sizeof(glm::vec2), &data[0], GL_STATIC_DRAW);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}

template <>
void BufferObject<glm::vec2>::UploadSubData(vector<glm::vec2> subdata,uint32_t offset)
{
    glBindBuffer(GL_ARRAY_BUFFER, this->Id);
    glBufferSubData(GL_ARRAY_BUFFER, offset*sizeof(glm::vec2), subdata.size()*sizeof(glm::vec2), &subdata[0]);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);
}
