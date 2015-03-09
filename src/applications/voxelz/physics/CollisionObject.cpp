#include "Precomp.h"

#include "CollisionObject.h"

void CollisionObject::Center(const void *_obj, ccd_vec3_t *center)
{
    CollisionObject *obj = (CollisionObject *)_obj;
    ccd_vec3_t cnt;
    cnt.v[0]=obj->_colShape.GetCenter().x;
    cnt.v[1]=obj->_colShape.GetCenter().y;
    cnt.v[2]=obj->_colShape.GetCenter().z;
    ccdVec3Copy(center, &cnt);
}

void CollisionObject::Support(const void *_obj, const ccd_vec3_t *_dir, ccd_vec3_t *vec)
{
    // assume that obj_t is user-defined structure that holds info about
    // object (in this case box: x, y, z, pos, quat - dimensions of box,
    // position and rotation)
    CollisionObject *obj = (CollisionObject *)_obj;
    ccd_vec3_t dir;

    // apply rotation on direction vector
    ccdVec3Copy(&dir, _dir);

    // compute support point in specified direction
    ccdVec3Set(vec, ccdSign(ccdVec3X(&dir)) * obj->_colShape.GetHalfSize().x,
               ccdSign(ccdVec3Y(&dir)) * obj->_colShape.GetHalfSize().y ,
               ccdSign(ccdVec3Z(&dir)) * obj->_colShape.GetHalfSize().z);

    // transform support point according to position and rotation of object
    ccdVec3Add(vec, &obj->_pos);
}
