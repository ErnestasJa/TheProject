#ifndef AABB_H
#define AABB_H

#include "utility/Vector.h"

class AABB
{
public:
    AABB();
    AABB(const glm::vec3 & center, const glm::vec3 & halfsize);
    virtual ~AABB();

    void Reset(const glm::vec3 &point);
    void AddPoint(const glm::vec3 &point);
    void CalculatePoints();
    bool ContainsPoint(const glm::vec3 &point) const;
    bool IntersectsWith(const AABB &other) const;
    float SweepCollidesWith(const AABB & other, const glm::vec3 & vel, glm::vec3 & normal) const;
    bool IntersectsWith(const glm::vec3 & aabbCenter, const glm::vec3 & aabbHalfsize) const;
    bool CollidesWithRay(const glm::vec3 & rayStart, const glm::vec3 & rayInverseDirection) const;
    void Translate(const glm::vec3 &point);
    void SetCenter(const glm::vec3 &point);

    glm::vec3 GetPoint(uint32_t i) const;
    vector<glm::vec3> GetPoints() const;
    glm::vec3 GetHalfSize() const;
    glm::vec3 GetCenter() const;
    glm::vec3 GetMin() const;
    glm::vec3 GetMax() const;

protected:
    glm::vec3 m_center,m_halfSize;
    vector<glm::vec3> points;
};

#endif // SG_AABB_H
