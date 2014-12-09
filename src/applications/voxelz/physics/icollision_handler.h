#ifndef ICOLLISIONHANDLER_H
#define ICOLLISIONHANDLER_H

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

class icollision_handler
{
public:
    virtual void on_body_collide(const btRigidBody*body1, const btRigidBody*body2)=0;
protected:
private:
};

#endif // ICOLLISIONHANDLER_H
