#pragma once

#include "../gui_button.h"

class toggle_group
{
    std::vector<gui_button*> elements;
    bool multitoggle;
    int m_current_toggle;

    bool toggle(gui_button* btn)
    {
        bool success=false;
        for(gui_button* el:elements)
        {
            if(this->multitoggle)
            {
                if(el==btn)
                {
                    el->set_toggled(!el->is_toggled());
                    return true;
                }
            }
            else
            {
                if(el==btn)
                {
                    el->set_toggled(true);
                    success=true;
                }
                else
                    el->set_toggled(false);
            }
        }
        return success;
    }
public:
    toggle_group(bool multitoggle=false,int deftoggle=0)
    {
        this->multitoggle=multitoggle;
    }

    toggle_group(std::vector<gui_button*> predef,bool multitoggle=false,int deftoggle=0)
    {
        this->multitoggle=multitoggle;
        elements=predef;
    }

    void add(gui_button* btn)
    {
        elements.push_back(btn);
    }

    gui_button* get_toggle()
    {
        return elements[m_current_toggle];
    }

    void set_toggle(int elem)
    {
        if(toggle(elements[elem]))
        {
            m_current_toggle=elem;
        }
    }
};
