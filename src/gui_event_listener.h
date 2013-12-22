#pragma once
#include "gui_event.h"

class gui_event_listener
{
public:
    virtual ~gui_event_listener() {}
    virtual void on_event(gui_event e)=0;
};
