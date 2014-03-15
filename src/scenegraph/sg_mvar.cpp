#include "precomp.h"
#include "sg_mvar.h"
#include "opengl/shader.h"

namespace sg
{

template <>
void sg_mvar<int32_t>::set(int32_t binding_index) const
{
    glUniform1i(binding_index,value);
}


template <>
void sg_mvar<float>::set(int32_t binding_index) const
{
    glUniform1f(binding_index,value);
}

template <>
void sg_mvar<glm::mat4x4>::set(int32_t binding_index) const
{
    glUniformMatrix4fv(binding_index, 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void sg_mvar<glm::mat3x3>::set(int32_t binding_index) const
{
    glUniformMatrix3fv(binding_index, 1, GL_FALSE, glm::value_ptr(value));
}


template <>
void sg_mvar<glm::vec3>::set(int32_t binding_index) const
{
    glUniform3fv(binding_index, 1, glm::value_ptr(value));
}

}
