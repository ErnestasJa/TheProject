#include "precomp.h"
#include "sg_mvar.h"
#include "opengl/shader.h"

namespace sg
{

template <>
void sg_mvar<int32_t>::set() const
{
    glUniform1i(binding_index,value);

    if(binding_index==-1)
        throw binding_index;
}


template <>
void sg_mvar<float>::set() const
{
    glUniform1f(binding_index,value);

    if(binding_index==-1)
        throw binding_index;
}

template <>
void sg_mvar<glm::mat4x4>::set() const
{
    glUniformMatrix4fv(binding_index, 1, GL_FALSE, glm::value_ptr(value));

    if(binding_index==-1)
        throw binding_index;
}

template <>
void sg_mvar<glm::mat3x3>::set() const
{
    glUniformMatrix3fv(binding_index, 1, GL_FALSE, glm::value_ptr(value));

    if(binding_index==-1)
        throw binding_index;
}


template <>
void sg_mvar<glm::vec3>::set() const
{
    glUniform3fv(binding_index, 1, glm::value_ptr(value));

    if(binding_index==-1)
        throw binding_index;
}

///binding index as param
template <>
void sg_mvar<int32_t>::set(int32_t bindex) const
{
    glUniform1i(bindex,value);
}


template <>
void sg_mvar<float>::set(int32_t bindex) const
{
    glUniform1f(bindex,value);
}

template <>
void sg_mvar<glm::mat4x4>::set(int32_t bindex) const
{
    glUniformMatrix4fv(bindex, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void sg_mvar<glm::mat3x3>::set(int32_t bindex) const
{
    glUniformMatrix3fv(bindex, 1, GL_FALSE, glm::value_ptr(value));
}


template <>
void sg_mvar<glm::vec3>::set(int32_t bindex) const
{
    glUniform3fv(bindex, 1, glm::value_ptr(value));
}

}
