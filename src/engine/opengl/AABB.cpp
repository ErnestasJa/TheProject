#include "Precomp.h"
#include "AABB.h"

AABB::AABB(): m_halfSize(0.5)
{
    //ctor
    points.resize(8);
    CalculatePoints();
}

AABB::AABB(const glm::vec3 & center, const glm::vec3 & halfSize):m_center(center), m_halfSize(halfSize)
{
    //ctor
    points.resize(8);
    CalculatePoints();
}

AABB::~AABB()
{
    //dtor
}

void AABB::Reset(const glm::vec3 &point)
{
    m_center = point;
    m_halfSize = glm::vec3(0);
}

void AABB::AddPoint(const glm::vec3 &point)
{
    glm::vec3 mi(m_center - m_halfSize), mx(m_center+m_halfSize);

    #define DOMIN(axis) mi.axis = point.axis < mi.axis ? (point.axis) : (mi.axis)
    #define DOMAX(axis) mx.axis = point.axis > mx.axis ? (point.axis) : (mx.axis)

    DOMIN(x);DOMIN(y);DOMIN(z);
    DOMAX(x);DOMAX(y);DOMAX(z);

    m_halfSize = glm::abs((mx-mi)*0.5f);
    m_center = (mi + mx) *0.5f;
}

void AABB::Translate(const glm::vec3 &point)
{
    m_center+=point;
}

bool AABB::ContainsPoint(const glm::vec3 & point) const
{
    glm::vec3 distance( m_center - point );

    return ( glm::abs( distance.x ) <= m_halfSize.x ) &
            ( glm::abs( distance.y ) <= m_halfSize.y ) &
            ( glm::abs( distance.z ) <= m_halfSize.z );
}

bool AABB::IntersectsWith(const AABB &other) const
{
   return (glm::abs(m_center.x - other.GetCenter().x) <= m_halfSize.x + other.GetHalfSize().x)
        &&(glm::abs(m_center.y - other.GetCenter().y) <= m_halfSize.y + other.GetHalfSize().y)
        &&(glm::abs(m_center.z - other.GetCenter().z) <= m_halfSize.z + other.GetHalfSize().z);
}

float AABB::SweepCollidesWith(const AABB & other, const glm::vec3 & vel, glm::vec3 & normal) const
{
    glm::vec3 invEntry, invExit;
    glm::vec3 p1s, p1e, p2s, p2e;
    p1s=this->m_center-this->m_halfSize;
    p1e=this->m_center+this->m_halfSize;
    p2s=other.m_center-other.m_halfSize;
    p2e=other.m_center+other.m_halfSize;

    #define INV_ENTRY_EXIT(axis)\
    if (vel.axis > 0.0f)\
    {\
    invEntry.axis = p2s.axis - p1e.axis;\
    invExit.axis = p2e.axis - p1s.axis;\
    }\
    else\
    {\
    invEntry.axis = p2e.axis - p1s.axis;\
    invExit.axis = p2s.axis - p1e.axis;\
    }

    INV_ENTRY_EXIT(x)
    INV_ENTRY_EXIT(y)
    INV_ENTRY_EXIT(z)
    #undef INV_ENTRY_EXIT
    // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
    glm::vec3 entry, exit;
    #define ENTRY_EXIT(axis)\
    if (vel.axis == 0.0f)\
    {\
    entry.axis = -std::numeric_limits<float>::infinity();\
    exit.axis = std::numeric_limits<float>::infinity();\
    }\
    else\
    {\
    entry.axis = invEntry.axis / vel.axis;\
    exit.axis = invExit.axis / vel.axis;\
    }
    ENTRY_EXIT(x)
    ENTRY_EXIT(y)
    ENTRY_EXIT(z)
    #undef ENTRY_EXIT
    // find the earliest/latest times of collision
    float entryTime = std::max(std::max(entry.x, entry.y),entry.z);
    float exitTime = std::min(std::min(exit.x, exit.y),exit.z);
    // if there was no collision
    if (entryTime > exitTime || (entry.x < 0.0f && entry.y < 0.0f && entry.z < 0.0f) || (entry.x > 1.0f || entry.y > 1.0f || entry.z > 1.0f))
    {
        normal.x = 0.0f;
        normal.y = 0.0f;
        normal.z = 0.0f;
        return 1.0f;
    }
    else // if there was a collision
    {
    // calculate normal of collided surface
        if (entry.x > entry.y && entry.x > entry.z)
        {
            if (invEntry.x < 0.0f)
            {
                normal.x = 1.0f;
                normal.y = 0.0f;
                normal.z = 0.0f;
            }
            else
            {
                normal.x = -1.0f;
                normal.y = 0.0f;
                normal.z = 0.0f;
            }
        }
        else if(entry.y > entry.x && entry.y > entry.z)
        {
            if (invEntry.y < 0.0f)
            {
                normal.x = 0.0f;
                normal.y = 1.0f;
                normal.z = 0.0f;
            }
            else
            {
                normal.x = 0.0f;
                normal.y = -1.0f;
                normal.z = 0.0f;
            }
        }
        else
        {
            if (invEntry.z < 0.0f)
            {
                normal.x = 0.0f;
                normal.y = 0.0f;
                normal.z = 1.0f;
            }
            else
            {
                normal.x = 0.0f;
                normal.y = 0.0f;
                normal.z = -1.0f;
            }
        }
// return the time of collision
        return entryTime;
    }
}

bool AABB::IntersectsWith(const glm::vec3 & center, const glm::vec3 & halfsize) const
{
    return (glm::abs(m_center.x - center.x) <= m_halfSize.x + halfsize.x)
        &&(glm::abs(m_center.y - center.y) <= m_halfSize.y + halfsize.y)
        &&(glm::abs(m_center.z - center.z) <= m_halfSize.z + halfsize.z);
}

bool AABB::CollidesWithRay(const glm::vec3 & rayStart, const glm::vec3 & rayDirectionInverse) const
{
    double tx1 = ((m_center.x-m_halfSize.x) - rayStart.x)*rayDirectionInverse.x;
    double tx2 = ((m_center.x+m_halfSize.x) - rayStart.x)*rayDirectionInverse.x;

    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);

    double ty1 = ((m_center.y-m_halfSize.y) - rayStart.y)*rayDirectionInverse.y;
    double ty2 = ((m_center.y+m_halfSize.y) - rayStart.y)*rayDirectionInverse.y;

    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    double tz1 = ((m_center.z-m_halfSize.z) - rayStart.z)*rayDirectionInverse.z;
    double tz2 = ((m_center.z+m_halfSize.z) - rayStart.z)*rayDirectionInverse.z;

    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= tmin;
}

glm::vec3 AABB::GetPoint(uint32_t i) const
{
    return points[i];
}

vector<glm::vec3> AABB::GetPoints() const
{
    return points;
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
    return m_center-m_halfSize;
}

glm::vec3 AABB::GetMax() const
{
    return m_center+m_halfSize;
}

void AABB::SetCenter(const glm::vec3 &point)
{
    m_center=point;
}

void AABB::CalculatePoints()
{
    float w=m_halfSize.x*2.0f,h=m_halfSize.y*2.0f,l=m_halfSize.z*2.0f;

    points[0]=m_center-m_halfSize;
    points[1]=m_center-glm::vec3(m_halfSize.x,0,0);
    points[2]=m_center-glm::vec3(0,m_halfSize.y,0);
    points[3]=m_center-glm::vec3(0,0,m_halfSize.z);

    points[4]=m_center+glm::vec3(0,0,m_halfSize.z);
    points[5]=m_center+glm::vec3(0,m_halfSize.y,0);
    points[6]=m_center+glm::vec3(m_halfSize.x,0,0);
    points[7]=m_center+m_halfSize;
}
