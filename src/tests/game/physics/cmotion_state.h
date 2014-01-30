#ifndef CMOTIONSTATE_H_INCLUDED
#define CMOTIONSTATE_H_INCLUDED

#include "scenegraph/sg_objects.h"

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

        trans.getOpenGLMatrix(&m_obj->get_transform()[0][0]);
    }

    virtual ~cmotion_state()
    {
    }

    virtual void getWorldTransform(btTransform &worldTrans) const
    {
        worldTrans = m_trans;
    }

    virtual void setWorldTransform(const btTransform &worldTrans)
    {
        if(!m_obj) return;

        worldTrans.getOpenGLMatrix(&m_obj->get_transform()[0][0]);
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
