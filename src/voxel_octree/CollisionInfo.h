#ifndef COLLISIONINFO_H_INCLUDED
#define COLLISIONINFO_H_INCLUDED

struct CollisionInfo
{
    boost::container::list<MNode> nodes;
    glm::vec3 normal;
};

#endif // COLLISIONINFO_H_INCLUDED
