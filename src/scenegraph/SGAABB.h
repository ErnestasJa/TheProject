#ifndef SG_AABB_H
#define SG_AABB_H


class sg_aabb
{
public:
    sg_aabb();
    sg_aabb(glm::vec3 min_point, glm::vec3 max_point);
    virtual ~sg_aabb();

    void reset(glm::vec3 point);
    void add_point(glm::vec3 point);
    bool is_point_inside(glm::vec3 point) const;

    glm::vec3 get_extent() const;
    glm::vec3 get_min() const;
    glm::vec3 get_max() const;

protected:
    glm::vec3 m_min_point, m_max_point;
};

#endif // SG_AABB_H
