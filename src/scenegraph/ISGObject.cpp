#include "Precomp.h"
#include "ISGObject.h"
#include "ISGRenderQueue.h"
#include "SGScenegraph.h"

namespace sg
{

isg_object::isg_object(SGScenegraph * sg):m_scenegraph(sg), m_scale(1,1,1), m_rotation(1,0,0,0)
{
    m_flags=0;
    update_absolute_transform();
}

isg_object::~isg_object()
{
    //dtor
}

const glm::mat4x4 & isg_object::get_absolute_transform()
{
    if(m_flags&SGOF_TRANSFORM_OUTDATED)
    {
        update_absolute_transform();
    }

    return m_absolute_transform;
}

std::string & isg_object::GetName()
{
    return m_name;
}

void isg_object::SetName(const std::string & name)
{
    m_name = name;
}

const sg_aabb & isg_object::get_aabb()
{
    return m_aabb;
}

const glm::vec3 &  isg_object::get_position() const
{
    return m_position;
}

const glm::quat &  isg_object::get_rotation() const
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

void isg_object::set_rotation(const glm::quat & v)
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
    glm::mat4x4 rt = glm::translate(glm::mat4(),m_position) * glm::toMat4(m_rotation) * glm::scale(glm::mat4(),m_scale);
    return rt;
}

void isg_object::update_absolute_transform()
{
    m_absolute_transform = get_relative_transform();

    rmbit(m_flags,SGOF_TRANSFORM_OUTDATED);
}

void isg_object::update(float delta_time)
{

}

bool isg_object::register_for_rendering()
{
    sg_render_queue_ptr rq = this->m_scenegraph->get_render_queue();
    rq->add_object(this);

    return true;
}

}
