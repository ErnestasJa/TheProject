#ifndef COLLISIONOBJECT_H
#define COLLISIONOBJECT_H


class CollisionObject
{
    public:
        CollisionObject();
        virtual ~CollisionObject();

        static void Support(const void *obj, const ccd_vec3_t *dir, ccd_vec3_t *vec);
        static void Center(const void *_obj, ccd_vec3_t *center);
    protected:
    private:
};

void CollisionObject::Center(const void *_obj, ccd_vec3_t *center)
{
    AABB *obj = (AABB *)_obj;
    ccd_vec3_t cnt;
    cnt.x=obj->GetCenter().x;
    cnt.y=obj->GetCenter().y;
    cnt.z=obj->GetCenter().z;
    ccdVec3Copy(center, &cnt);
}

void CollisionObject::Support(const void *obj, const ccd_vec3_t *dir, ccd_vec3_t *vec)
{
    // assume that obj_t is user-defined structure that holds info about
    // object (in this case box: x, y, z, pos, quat - dimensions of box,
    // position and rotation)
    AABB *obj = (AABB *)_obj;
    ccd_vec3_t dir;

    // apply rotation on direction vector
    ccdVec3Copy(&dir, _dir);

    // compute support point in specified direction
    ccdVec3Set(vec, ccdSign(ccdVec3X(&dir)) * obj->GetHalfSize().x * CCD_REAL(0.5),
               ccdSign(ccdVec3Y(&dir)) * obj->GetHalfSize().y * CCD_REAL(0.5),
               ccdSign(ccdVec3Z(&dir)) * obj->GetHalfSize().z * CCD_REAL(0.5));

    // transform support point according to position and rotation of object
    ccdVec3Add(vec, &obj->pos);
}

#endif // COLLISIONOBJECT_H
