#include "Precomp.h"
#include "AABB.h"

AABB::AABB(): m_min(-1,-1,-1), m_max(1,1,1)
{
    //ctor
}

AABB::AABB(const glm::vec3 & min, const glm::vec3 & max): m_min(min), m_max(max)
{
    //ctor
}

AABB::~AABB()
{
    //dtor
}

void AABB::Reset(const glm::vec3 & point)
{
    m_min = m_max = point;
}

void AABB::AddPoint(const glm::vec3 & point)
{
    if (point.x>m_max.x) m_max.x = point.x;
    if (point.y>m_max.y) m_max.y = point.y;
    if (point.z>m_max.z) m_max.z = point.z;

    if (point.x<m_min.x) m_min.x = point.x;
    if (point.y<m_min.y) m_min.y = point.y;
    if (point.z<m_min.z) m_min.z = point.z;
}

void AABB::Translate(const glm::vec3 & offset)
{
    m_min+=offset;
    m_max+=offset;
}

bool AABB::IsPointInside(const glm::vec3 & point) const
{
    return
    m_min.x <= point.x &&
    m_min.y <= point.y &&
    m_min.z <= point.z &&
    m_max.x >= point.x &&
    m_max.y >= point.y &&
    m_max.z >= point.z;
}

bool AABB::CollidesWith(const AABB & other) const
{
    return (m_min.x <= other.m_max.x && m_min.y <= other.m_max.y && m_min.z <= other.m_max.z &&
            m_max.x >= other.m_min.x && m_max.y >= other.m_min.y && m_max.z >= other.m_min.z);
}

bool AABB::CollidesWith(const glm::vec3 & aabb_min, const glm::vec3 & aabb_max) const
{
    return (m_min.x <= aabb_max.x && m_min.y <= aabb_max.y && m_min.z <= aabb_max.z &&
            m_max.x >= aabb_min.x && m_max.y >= aabb_min.y && m_max.z >= aabb_min.z);
}

bool AABB::CollidesWithRay(const glm::vec3 & rayStart, const glm::vec3 & rayDirectionInverse) const
{
    double tx1 = (m_min.x - rayStart.x)*rayDirectionInverse.x;
    double tx2 = (m_max.x - rayStart.x)*rayDirectionInverse.x;

    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);

    double ty1 = (m_min.y - rayStart.y)*rayDirectionInverse.y;
    double ty2 = (m_max.y - rayStart.y)*rayDirectionInverse.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    double tz1 = (m_min.z - rayStart.z)*rayDirectionInverse.z;
    double tz2 = (m_max.z - rayStart.z)*rayDirectionInverse.z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= tmin;
}

glm::vec3 AABB::GetExtent() const
{
    return m_max - m_min;
}

const glm::vec3 & AABB::GetMin() const
{
    return m_min;
}

const glm::vec3 & AABB::GetMax() const
{
    return m_max;
}
