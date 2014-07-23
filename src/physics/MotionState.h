#ifndef CMOTIONSTATE_H_INCLUDED
#define CMOTIONSTATE_H_INCLUDED

#include "scenegraph/SGObjects.h"

#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

///you could also add some other things to this motion state,
///like game object class that is related to the body using this motion state.
///this way you won't need to use 'user pointer'.

class cmotion_state: public btMotionState
{
public:
    cmotion_state(const btTransform & trans, sg::sg_object_ptr obj)
    {
        m_obj = obj;
        m_trans = trans;

        btQuaternion quat = trans.getRotation();
        btVector3 pos = trans.getOrigin();

        m_obj->set_rotation(glm::quat(quat.getW(),quat.getX(),quat.getY(),quat.getZ()));
        m_obj->set_position(glm::vec3(pos.getX(),pos.getY(),pos.getZ()));
        m_obj->update_absolute_transform();
    }

    virtual ~cmotion_state()
    {
    }

    virtual void getWorldTransform(btTransform &worldTrans) const
    {
        worldTrans = m_trans;
    }

    virtual void setWorldTransform(const btTransform &trans)
    {
        if(!m_obj) return;

        btQuaternion quat = trans.getRotation();
        btVector3 pos = trans.getOrigin();

        m_obj->set_rotation(glm::quat(quat.getW(),quat.getX(),quat.getY(),quat.getZ()));
        m_obj->set_position(glm::vec3(pos.getX(),pos.getY(),pos.getZ()));
        m_obj->update_absolute_transform();
    }

    sg::sg_object_ptr get_sg_obj()
    {
        return m_obj;
    }

protected:
    sg::sg_object_ptr m_obj;
    btTransform m_trans;

};

#endif // CMOTIONSTATE_H_INCLUDED
