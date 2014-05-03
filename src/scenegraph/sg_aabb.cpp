#include "precomp.h"
#include "sg_aabb.h"

sg_aabb::sg_aabb(): m_min_point(-1,-1,-1), m_max_point(1,1,1)
{
    //ctor
}

sg_aabb::sg_aabb(glm::vec3 min_point, glm::vec3 max_point): m_min_point(min_point), m_max_point(max_point)
{
    //ctor
}

sg_aabb::~sg_aabb()
{
    //dtor
}

void sg_aabb::reset(glm::vec3 point)
{
    m_min_point = m_max_point = point;
}

void sg_aabb::add_point(glm::vec3 point)
{
    if (point.x>m_max_point.x) m_max_point.x = point.x;
    if (point.y>m_max_point.y) m_max_point.y = point.y;
    if (point.z>m_max_point.z) m_max_point.z = point.z;

    if (point.x<m_min_point.x) m_min_point.x = point.x;
    if (point.y<m_min_point.y) m_min_point.y = point.y;
    if (point.z<m_min_point.z) m_min_point.z = point.z;
}

bool sg_aabb::is_point_inside(glm::vec3 point) const
{
    return
    m_min_point.x <= point.x &&
    m_min_point.y <= point.y &&
    m_min_point.z <= point.z &&
    m_max_point.x >= point.x &&
    m_max_point.y >= point.y &&
    m_max_point.z >= point.z;
}

glm::vec3 sg_aabb::get_extent() const
{
    return m_max_point - m_min_point;
}

glm::vec3 sg_aabb::get_min() const
{
    return m_min_point;
}

glm::vec3 sg_aabb::get_max() const
{
    return m_max_point;
}
