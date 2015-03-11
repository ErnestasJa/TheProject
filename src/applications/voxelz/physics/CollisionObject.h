#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H

#include <ccd/ccd.h>
#include "opengl/AABB.h"

inline ccd_vec3_t GLMtoCCD(const glm::vec3 &in)
{
    ccd_vec3_t ret;
    ret.v[0]=in.x;
    ret.v[1]=in.y;
    ret.v[2]=in.z;

    return ret;
}

inline glm::vec3 CCDtoGLM(const ccd_vec3_t &in)
{
    return glm::vec3(in.v[0],in.v[1],in.v[2]);
}

struct CollisionInfo
{
    bool colliding;
    ccd_real_t depth;
    ccd_vec3_t dir,pos;

    CollisionInfo()
    {
        colliding=false;
        depth=0;
        ccdVec3Set(&dir,0,0,0);
        ccdVec3Set(&pos,0,0,0);
    }
};

class CollisionObject
{
public:
    CollisionObject(const glm::vec3 &center,const glm::vec3 &halfsize)
    {
        _colShape=AABB(center,halfsize);
        _pos=GLMtoCCD(center);
    }
    virtual ~CollisionObject(){};

    static void Support(const void *obj, const ccd_vec3_t *dir, ccd_vec3_t *vec);
    static void Center(const void *_obj, ccd_vec3_t *center);

    void Translate(const glm::vec3& trans)
    {
        _colShape.Translate(trans);
        ccd_vec3_t asd=GLMtoCCD(trans);
        ccdVec3Add(&_pos,&asd);
    }

    ccd_vec3_t _pos,_dir;
    AABB _colShape;
protected:
private:
};

inline bool MPRCollide(CollisionObject * obj1, CollisionObject * obj2)
{
    ccd_t ccd;
    CCD_INIT(&ccd);

    ccd.support1=obj1->Support;
    ccd.support2=obj2->Support;
    ccd.center1=obj1->Center;
    ccd.center2=obj2->Center;
    ccd.mpr_tolerance=0.000001;
    ccd.max_iterations=100;

    return ccdMPRIntersect(obj1,obj2,&ccd);
}

inline CollisionInfo MPRPenetration(CollisionObject * obj1, CollisionObject * obj2)
{
    ccd_t ccd;
    CCD_INIT(&ccd);

    ccd.support1=obj1->Support;
    ccd.support2=obj2->Support;
    ccd.center1=obj1->Center;
    ccd.center2=obj2->Center;
    ccd.mpr_tolerance=0.000001;
    ccd.max_iterations=100;

    CollisionInfo ret;

    ret.colliding=ccdMPRPenetration(obj1,obj2,&ccd,&ret.depth,&ret.dir,&ret.pos)+1;

    return ret;
}

#endif // COLLISIONOBJECT_H
