#ifndef AABB_H
#define AABB_H

class AABB
{
public:
    AABB();
    AABB(glm::vec3 min_point, glm::vec3 max_point);
    virtual ~AABB();

    void Reset(const glm::vec3 &point);
    void AddPoint(const glm::vec3 &point);
    void CalculatePoints();
    bool ContainsPoint(const glm::vec3 &point) const;
    bool IntersectsWith(const AABB &other) const;

    glm::vec3 GetPoint(uint32_t i) const;
    glm::vec3 GetHalfSize() const;
    glm::vec3 GetCenter() const;
    glm::vec3 GetMin() const;
    glm::vec3 GetMax() const;

protected:

    glm::vec3 m_min_point,m_max_point,m_center,m_halfSize,m_invalid;
    glm::vec3 points[8];
};

#endif // SG_AABB_H
