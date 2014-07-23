#ifndef COLLISIONFILTERRESULTCALLBACKONLYTHIS_H_INCLUDED
#define COLLISIONFILTERRESULTCALLBACKONLYTHIS_H_INCLUDED

///Ray test callback that should get only body that mathces the filter.
///Not much tested but I think it works.
struct	CollisionFilterResultCallbackOnlyThis : public btCollisionWorld::RayResultCallback
{
    CollisionFilterResultCallbackOnlyThis(const btVector3&	rayFromWorld,const btVector3&	rayToWorld, short int filter)
        :m_rayFromWorld(rayFromWorld),
         m_rayToWorld(rayToWorld),
         m_filter(filter)
    {
    }



    btVector3	m_rayFromWorld;//used to calculate hitPointWorld from hitFraction
    btVector3	m_rayToWorld;

    btVector3	m_hitNormalWorld;
    btVector3	m_hitPointWorld;

    short int   m_filter;

    virtual	btScalar	addSingleResult(btCollisionWorld::LocalRayResult& rayResult,bool normalInWorldSpace)
    {
        //caller already does the filter on the m_closestHitFraction
        btAssert(rayResult.m_hitFraction <= m_closestHitFraction);

        /*if(!m_collisionObject)
        {

            m_collisionObject = rayResult.m_collisionObject;
            if((rayResult.m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup&m_filter)==m_filter)
                m_closestHitFraction = 0;
            else
                m_closestHitFraction = rayResult.m_hitFraction;

        }
        else*/ if((rayResult.m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup&m_filter)==m_filter)
        {
            m_closestHitFraction = rayResult.m_hitFraction;
            m_collisionObject = rayResult.m_collisionObject;
        }
        /*else if(!((m_collisionObject->getBroadphaseHandle()->m_collisionFilterGroup&m_filter)==m_filter))
        {
            m_closestHitFraction = rayResult.m_hitFraction;
            m_collisionObject = rayResult.m_collisionObject;
        }*/


        if (normalInWorldSpace)
        {
            m_hitNormalWorld = rayResult.m_hitNormalLocal;
        }
        else
        {
            ///need to transform normal into worldspace
            m_hitNormalWorld = m_collisionObject->getWorldTransform().getBasis()*rayResult.m_hitNormalLocal;
        }
        m_hitPointWorld.setInterpolate3(m_rayFromWorld,m_rayToWorld,rayResult.m_hitFraction);

        return m_closestHitFraction;
    }
};

#endif // COLLISIONFILTERRESULTCALLBACKONLYTHIS_H_INCLUDED
