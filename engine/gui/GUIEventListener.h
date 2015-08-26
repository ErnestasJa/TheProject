#pragma once
#include "GUIEvent.h"

class GUIEventListener
{
public:
    virtual ~GUIEventListener() {}
    virtual bool OnEvent(const GUIEvent & e)=0;
};
