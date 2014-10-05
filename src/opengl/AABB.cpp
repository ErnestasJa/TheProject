#include "Precomp.h"
#include "AABB.h"

AABB::AABB(): m_min_point(-1),m_max_point(1)
{
    //ctor
    CalculatePoints();
}

AABB::AABB(glm::vec3 min_point, glm::vec3 max_point):m_min_point(min_point),m_max_point(max_point)
{
    //ctor
    CalculatePoints();
}

AABB::~AABB()
{
    //dtor
}

void AABB::Reset(const glm::vec3 &point)
{
    m_min_point = m_max_point = point;
}

void AABB::AddPoint(const glm::vec3 &point)
{
    if (point.x>m_max_point.x) m_max_point.x = point.x;
    if (point.y>m_max_point.y) m_max_point.y = point.y;
    if (point.z>m_max_point.z) m_max_point.z = point.z;

    if (point.x<m_min_point.x) m_min_point.x = point.x;
    if (point.y<m_min_point.y) m_min_point.y = point.y;
    if (point.z<m_min_point.z) m_min_point.z = point.z;
}

bool AABB::ContainsPoint(const glm::vec3 &point) const
{
    glm::vec3 distance( m_center - point );

    return ( glm::abs( distance.x ) <= m_halfSize.x ) &
            ( glm::abs( distance.y ) <= m_halfSize.y ) &
            ( glm::abs( distance.z ) <= m_halfSize.z );
}

bool AABB::IntersectsWith(const AABB &other) const
{
   //check the X axis
   if(glm::abs(m_center.x - other.GetCenter().x) <= m_halfSize.x + other.GetHalfSize().x)
   {
      //check the Y axis
      if(glm::abs(m_center.y - other.GetCenter().y) <= m_halfSize.y + other.GetHalfSize().y)
      {
          //check the Z axis
          if(glm::abs(m_center.z - other.GetCenter().z) <= m_halfSize.z + other.GetHalfSize().z)
          {
             return true;
          }
      }
   }

   return false;
}

glm::vec3 AABB::GetPoint(uint32_t i) const
{
    if(i>7) return m_invalid;
    return points[i];
}

glm::vec3 AABB::GetHalfSize() const
{
    return m_halfSize;
}

glm::vec3 AABB::GetCenter() const
{
    return m_center;
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
    m_center=(m_min_point+m_max_point)/2.f;
    m_halfSize=glm::abs(m_max_point-m_center);

    float w=m_halfSize.x,h=m_halfSize.y,l=m_halfSize.z;

    points[0]=m_center;
    points[1]=m_center+glm::vec3(w,0,0);
    points[2]=m_center+glm::vec3(0,h,0);
    points[3]=m_center+glm::vec3(0,0,l);

    points[4]=m_center-glm::vec3(l,0,0);
    points[5]=m_center-glm::vec3(0,h,0);
    points[6]=m_center-glm::vec3(0,0,w);
    points[7]=m_center;
}
