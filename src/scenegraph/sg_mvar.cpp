#include "precomp.h"
#include "sg_mvar.h"
#include "opengl/shader.h"

namespace sg
{

template <>
void sg_mvar<float>::set(shader_ptr pshader) const
{
    glUniform1f(pshader->getparam(name),value);
}


template <>
void sg_mvar<glm::mat4x4>::set(shader_ptr pshader) const
{
    glUniformMatrix4fv(pshader->getparam(name), 1, GL_FALSE, glm::value_ptr(value));
}

template <>
void sg_mvar<glm::mat3x3>::set(shader_ptr pshader) const
{
    glUniformMatrix3fv(pshader->getparam(name), 1, GL_FALSE, glm::value_ptr(value));
}


template <>
void sg_mvar<glm::vec3>::set(shader_ptr pshader) const
{
    glUniform3fv(pshader->getparam(name), 1, glm::value_ptr(value));
}

}
