#include "Precomp.h"
#include "MVar.h"
#include "opengl/Shader.h"

template <>
void MVar<int32_t>::Set() const
{
    glUniform1i(binding_index,value);

    if(binding_index==-1)
        throw binding_index;
}


template <>
void MVar<float>::Set() const
{
    glUniform1f(binding_index,value);

    if(binding_index==-1)
        throw binding_index;
}

template <>
void MVar<glm::mat4x4>::Set() const
{
    glUniformMatrix4fv(binding_index, 1, GL_FALSE, glm::value_ptr(value));

    if(binding_index==-1)
        throw binding_index;
}

template <>
void MVar<glm::mat3x3>::Set() const
{
    glUniformMatrix3fv(binding_index, 1, GL_FALSE, glm::value_ptr(value));

    if(binding_index==-1)
        throw binding_index;
}


template <>
void MVar<glm::vec3>::Set() const
{
    glUniform3fv(binding_index, 1, glm::value_ptr(value));

    if(binding_index==-1)
        throw binding_index;
}

///binding index as param
template <>
void MVar<int32_t>::Set(int32_t bindex) const
{
    glUniform1i(bindex,value);
}


template <>
void MVar<float>::Set(int32_t bindex) const
{
    glUniform1f(bindex,value);
}

template <>
void MVar<glm::mat4x4>::Set(int32_t bindex) const
{
    glUniformMatrix4fv(bindex, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void MVar<glm::mat3x3>::Set(int32_t bindex) const
{
    glUniformMatrix3fv(bindex, 1, GL_FALSE, glm::value_ptr(value));
}


template <>
void MVar<glm::vec3>::Set(int32_t bindex) const
{
    glUniform3fv(bindex, 1, glm::value_ptr(value));
}
