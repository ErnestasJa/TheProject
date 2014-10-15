#include "Precomp.h"
#include "AABB.h"

AABB::AABB(): m_halfSize(0.5)
{
    //ctor
    CalculatePoints();
}

AABB::AABB(const glm::vec3 & center, const glm::vec3 & halfSize):m_center(center), m_halfSize(halfSize)
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
    m_center = point;
    m_halfSize = glm::vec3(0);
}

void AABB::AddPoint(const glm::vec3 &point)
{
    glm::vec3 mi(m_center - m_halfSize), mx(m_center+m_halfSize);
    float tmp;

    //max
    if (point.x > mx.x)
    {
        tmp = point.x - mx.x;
        m_halfSize.x += tmp;
        m_center.x += tmp*0.5f;
    }
    else if (point.x < mi.x)
    {
        tmp = mi.x - point.x;
        m_halfSize.x -= tmp;
        m_center.x -= tmp*0.5f;
    }

    if (point.y > mx.y)
    {
        tmp = point.y - mx.y;
        m_halfSize.y += tmp;
        m_center.y += tmp*0.5f;
    }
    else if (point.y < mi.y)
    {
        tmp = mi.y - point.y;
        m_halfSize.y -= tmp;
        m_center.y -= tmp*0.5f;
    }

    if (point.z > mx.z)
    {
        tmp = point.z - mx.z;
        m_halfSize.z += tmp;
        m_center.z += tmp*0.5f;
    }
    else if (point.z < mi.z)
    {
        tmp = mi.z - point.z;
        m_halfSize.z -= tmp;
        m_center.z -= tmp*0.5f;
    }
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

float AABB::SweepCollidesWith(const glm::vec3 & vel, const AABB & other, glm::vec3 & normal) const
{
    glm::vec3 invEntry, invExit;

    // find the distance between the objects on the near and far sides for both x and y
    if (vel.x > 0.0f)
    {
        invEntry.x = (other.m_center.x-other.m_halfSize.x) - (this->m_center.x + this->m_halfSize.x);
        invExit.x =  (other.m_center.x+other.m_halfSize.x) - (this->m_center.x - this->m_halfSize.x);
    }
    else
    {
        invEntry.x = (other.m_center.x+other.m_halfSize.x) - (this->m_center.x - this->m_halfSize.x);
        invExit.x =  (other.m_center.x-other.m_halfSize.x) - (this->m_center.x + this->m_halfSize.x);
    }

    if (vel.y > 0.0f)
    {
        invEntry.y = (other.m_center.y-other.m_halfSize.y) - (this->m_center.y + this->m_halfSize.y);
        invExit.y =  (other.m_center.y+other.m_halfSize.y) - (this->m_center.y - this->m_halfSize.y);
    }
    else
    {
        invEntry.y = (other.m_center.y+other.m_halfSize.y) - (this->m_center.y - this->m_halfSize.y);
        invExit.y =  (other.m_center.y-other.m_halfSize.y) - (this->m_center.y + this->m_halfSize.y);
    }

    if (vel.z > 0.0f)
    {
        invEntry.z = (other.m_center.z-other.m_halfSize.z) - (this->m_center.z + this->m_halfSize.z);
        invExit.z =  (other.m_center.z+other.m_halfSize.z) - (this->m_center.z - this->m_halfSize.y);
    }
    else
    {
        invEntry.z = (other.m_center.z+other.m_halfSize.z) - (this->m_center.z - this->m_halfSize.z);
        invExit.z =  (other.m_center.z-other.m_halfSize.z) - (this->m_center.z + this->m_halfSize.z);
    }

    // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
    glm::vec3 entry, exit;

    if (vel.x == 0.0f)
    {
        entry.x = -std::numeric_limits<float>::infinity();
        exit.x = std::numeric_limits<float>::infinity();
    }
    else
    {
        entry.x = invEntry.x / vel.x;
        exit.x = invExit.x / vel.x;
    }

    if (vel.y == 0.0f)
    {
        entry.y = -std::numeric_limits<float>::infinity();
        exit.y = std::numeric_limits<float>::infinity();
    }
    else
    {
        entry.y = invEntry.y / vel.y;
        exit.y = invExit.y / vel.y;
    }

    if (vel.z == 0.0f)
    {
        entry.z = -std::numeric_limits<float>::infinity();
        exit.z = std::numeric_limits<float>::infinity();
    }
    else
    {
        entry.z = invEntry.z / vel.z;
        exit.z = invExit.z / vel.z;
    }

    // find the earliest/latest times of collision
    float entryTime = std::max(std::max(entry.x, entry.y),entry.z);
    float exitTime = std::min(std::min(exit.x, exit.y),exit.z);

    // if there was no collision
    if (entryTime > exitTime || entry.x < 0.0f && entry.y < 0.0f && entry.z < 0.0f || entry.x > 1.0f || entry.y > 1.0f || entry.z > 1.0f)
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

glm::vec3 AABB::GetHalfSize() const
{
    return m_halfSize;
}

glm::vec3 AABB::GetCenter() const
{
    return m_center;
}

const glm::vec3 & AABB::GetMin() const
{
    return m_center-m_halfSize;
}

const glm::vec3 & AABB::GetMax() const
{
    return m_center+m_halfSize;
}

void AABB::CalculatePoints()
{
    float w=m_halfSize.x*2.0f,h=m_halfSize.y*2.0f,l=m_halfSize.z*2.0f;

    points[0]=m_center;
    points[1]=m_center+glm::vec3(w,0,0);
    points[2]=m_center+glm::vec3(0,h,0);
    points[3]=m_center+glm::vec3(0,0,l);

    points[4]=m_center-glm::vec3(l,0,0);
    points[5]=m_center-glm::vec3(0,h,0);
    points[6]=m_center-glm::vec3(0,0,w);
    points[7]=m_center;
}
