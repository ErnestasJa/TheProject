#include "Precomp.h"
#include "AABB.h"

AABB::AABB(): m_min_point(-1,-1,-1), m_max_point(1,1,1)
{
    //ctor
    CalculatePoints();
}

AABB::AABB(glm::vec3 min_point, glm::vec3 max_point): m_min_point(min_point), m_max_point(max_point)
{
    //ctor
    CalculatePoints();
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

glm::vec3 AABB::GetPoint(uint32_t i) const
{
    if(i>7) return m_invalid;
    return points[i];
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

void AABB::CalculatePoints()
{
    glm::vec3 ext=GetExtent();

    w=glm::abs(ext.x);
    h=glm::abs(ext.y);
    l=glm::abs(ext.z);

    points[0]=m_min_point;
    points[1]=m_min_point+glm::vec3(w,0,0);
    points[2]=m_min_point+glm::vec3(0,h,0);
    points[3]=m_min_point+glm::vec3(0,0,l);

    points[4]=m_max_point-glm::vec3(l,0,0);
    points[5]=m_max_point-glm::vec3(0,h,0);
    points[6]=m_max_point-glm::vec3(0,0,w);
    points[7]=m_max_point;
}
