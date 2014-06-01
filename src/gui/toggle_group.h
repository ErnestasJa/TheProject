#pragma once

#include "gui_button.h"

class toggle_group
{
    std::vector<gui_button*> elements;
    bool multitoggle;
public:
    toggle_group(bool multitoggle=false)
    {
        this->multitoggle=multitoggle;
    }

    toggle_group(std::vector<gui_button*> predef,bool multitoggle=false)
    {
        this->multitoggle=multitoggle;
        elements=predef;
    }

    void add(gui_button* btn)
    {
        elements.push_back(btn);
    }

    void toggle(gui_button* btn)
    {
        for(gui_button* el:elements)
        {
            if(this->multitoggle)
            {
                if(el==btn)
                    el->set_toggled(!el->is_toggled());
            }
            else
            {
                if(el==btn)
                    el->set_toggled(true);
                else
                    el->set_toggled(false);
            }
        }
    }
};
