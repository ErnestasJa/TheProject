#include "precomp.h"
#include "isg_object.h"

namespace sg
{

isg_object::isg_object(scenegraph * sg)
{
    m_scenegraph=sg;
}

isg_object::~isg_object()
{
    //dtor
}

const glm::mat4x4 & isg_object::get_absolute_transform()
{
    if(m_flags&SGOF_TRANSFORM_OUTDATED)
    {
        rmbit(m_flags,SGOF_TRANSFORM_OUTDATED);
        update_absolute_transform();
    }

    return m_absolute_transform;
}

std::string & isg_object::get_name()
{
    return m_name;
}

const sg_aabb & isg_object::get_aabb()
{
    return m_aabb;
}

const glm::vec3 &  isg_object::get_position() const
{
    return m_position;
}

const glm::vec3 &  isg_object::get_rotation() const
{
    return m_rotation;
}

const glm::vec3 &  isg_object::get_scale() const
{
    return m_scale;
}

void isg_object::set_position(const glm::vec3 & v)
{
    m_position = v;
    m_flags |= SGOF_TRANSFORM_OUTDATED;
}

void isg_object::set_rotation(const glm::vec3 & v)
{
    m_rotation = v;
    m_flags |= SGOF_TRANSFORM_OUTDATED;
}

void isg_object::set_scale(const glm::vec3 & v)
{
    m_scale = v;
    m_flags |= SGOF_TRANSFORM_OUTDATED;
}

glm::mat4x4 isg_object::get_relative_transform()
{
    glm::mat4x4 rt(1.0);
    rt = glm::rotate(rt,m_rotation.x,glm::vec3(1,0,0));
    rt = glm::rotate(rt,m_rotation.y,glm::vec3(0,1,0));
    rt = glm::rotate(rt,m_rotation.z,glm::vec3(0,0,1));
    rt = glm::translate(rt,m_position);
    return rt;
}

void isg_object::update_absolute_transform()
{
        m_absolute_transform = get_relative_transform();
}

}
