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

glm::mat4x4 & isg_object::get_transform()
{
    return m_transform;
}

std::string & isg_object::get_name()
{
    return m_name;
}
sg_aabb isg_object::get_aabb()
{
    return sg_aabb(glm::vec3(),glm::vec3());
}

}
