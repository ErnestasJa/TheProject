#ifndef AABB_H
#define AABB_H


class AABB
{
public:
    AABB();
    AABB(glm::vec3 min_point, glm::vec3 max_point);
    virtual ~AABB();

    void Reset(glm::vec3 point);
    void AddPoint(glm::vec3 point);
    bool IsPointInside(glm::vec3 point) const;

    glm::vec3 GetPoint(uint32_t i) const;
    glm::vec3 GetExtent() const;
    glm::vec3 GetMin() const;
    glm::vec3 GetMax() const;

protected:
    void CalculatePoints();

    glm::vec3 m_min_point, m_max_point, m_center, m_invalid;
    float w,h,l;
    glm::vec3 points[8];
};

#endif // SG_AABB_H
