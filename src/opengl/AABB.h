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
    bool is_point_inside(glm::vec3 point) const;

    glm::vec3 get_extent() const;
    glm::vec3 get_min() const;
    glm::vec3 get_max() const;

protected:
    glm::vec3 m_min_point, m_max_point;
};

#endif // SG_AABB_H
