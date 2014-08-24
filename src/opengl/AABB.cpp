#include "Precomp.h"
#include "AABB.h"

AABB::AABB(): m_min_point(-1,-1,-1), m_max_point(1,1,1)
{
    //ctor
}

AABB::AABB(glm::vec3 min_point, glm::vec3 max_point): m_min_point(min_point), m_max_point(max_point)
{
    //ctor
}

AABB::~AABB()
{
    //dtor
}

void AABB::Reset(glm::vec3 point)
{
    m_min_point = m_max_point = point;
}

void AABB::AddPoint(glm::vec3 point)
{
    if (point.x>m_max_point.x) m_max_point.x = point.x;
    if (point.y>m_max_point.y) m_max_point.y = point.y;
    if (point.z>m_max_point.z) m_max_point.z = point.z;

    if (point.x<m_min_point.x) m_min_point.x = point.x;
    if (point.y<m_min_point.y) m_min_point.y = point.y;
    if (point.z<m_min_point.z) m_min_point.z = point.z;
}

bool AABB::IsPointInside(glm::vec3 point) const
{
    return
    m_min_point.x <= point.x &&
    m_min_point.y <= point.y &&
    m_min_point.z <= point.z &&
    m_max_point.x >= point.x &&
    m_max_point.y >= point.y &&
    m_max_point.z >= point.z;
}

glm::vec3 AABB::GetExtent() const
{
    return m_max_point - m_min_point;
}

glm::vec3 AABB::GetMin() const
{
    return m_min_point;
}

glm::vec3 AABB::GetMax() const
{
    return m_max_point;
}
