#include "precomp.h"

#include "gui/gui_element.h"
#include "utility/logger.h"

gui_element::gui_element(int x, int y, int w, int h)
{
    absolute_rect = rect2d<int>(x, y, w, h);
    relative_rect = rect2d<int>(x, y, w, h);

    hovered = false;
    visible = true;
    focused = false;
    enabled = true;
    accept_events = true;

    this->parent = nullptr;
    this->event_listener = nullptr;
    this->environment = nullptr;
    set_name("gui_element_"+tostr(rand()%65535));
}

gui_element::~gui_element()
{

}

void gui_element::render()
{
    //will be overriden by everything
}

void gui_element::add_child(gui_element *e)
{
    for(gui_element *i : children)
        if(i==e)
            return;

    e->parent=this;
    children.push_back(e);
    update_absolute_pos();
    printf("ADD CHILD My bounds are: %i %i %i %i %i %i %i %i\n",e->absolute_rect.x,e->absolute_rect.y,e->absolute_rect.w,e->absolute_rect.h,e->relative_rect.x,e->relative_rect.y,e->relative_rect.w,e->relative_rect.h);
}

void gui_element::remove_child(gui_element *e)
{
    for(std::vector<gui_element*>::iterator i=children.begin(); i!=children.end(); i++)
        if((gui_element*)(&i)==e)
        {
            e->parent=nullptr;
            children.erase(i);
            return;
        }
}

void gui_element::bring_to_front(gui_element *e)
{
    if(children.size()>0)
    for(std::vector<gui_element*>::iterator i=children.begin(); i!=children.end(); i++)
        if(*i == e)
        {
            i=children.erase(i);
            children.push_back(e);
            return;
        }
        else
            i++;
}

void gui_element::render_children()
{
    for(gui_element *e : children)
    {
        if(e->is_visible())
            e->render();
    }
}

void gui_element::update_absolute_pos()
{
    if(this->parent!=nullptr)
    {
        this->relative_rect=this->parent->absolute_rect;
        this->absolute_rect=
            rect2d<int>(this->absolute_rect.x+this->relative_rect.x,
                            this->absolute_rect.y + this->relative_rect.y,
                            this->absolute_rect.w, this->absolute_rect.h);
    }
    for(gui_element *e : children)
        e->update_absolute_pos();
}

void gui_element::set_name(std::string name)
{
    this->name=name;
}

void gui_element::set_event_listener(gui_event_listener *listener)
{
    this->event_listener=listener;
}

void gui_element::set_enabled(bool b)
{
    this->enabled=b;
    if(children.size()>0)
        for(gui_element *e : children)
            e->set_enabled(true);
}

void gui_element::set_focused(bool b)
{
    this->focused=b;
}

void gui_element::set_visible(bool b)
{
    this->visible=b;
}

void gui_element::set_hovered(bool b)
{
    this->hovered=b;
}

void gui_element::set_listening(bool b)
{
    this->accept_events=b;
}

void gui_element::set_parent(gui_element *e)
{
    if(this->parent!=nullptr)
        this->parent->remove_child(this);
    if(e!=nullptr)
        e->add_child(this);
    else
        return;
}

bool gui_element::is_enabled()
{
    return this->enabled;
}

bool gui_element::is_focused()
{
    return this->focused;
}

bool gui_element::is_visible()
{
    return this->visible;
}

bool gui_element::is_hovered()
{
    return this->hovered;
}

bool gui_element::accepts_events()
{
    return this->accept_events;
}

gui_environment *gui_element::get_environment()
{
    return this->environment;
}

gui_element *gui_element::get_parent()
{
    return this->parent;
}

std::string gui_element::get_name()
{
    return this->name;
}

gui_element *gui_element::get_element_from_point(int x, int y)
{
    gui_element *ret=nullptr;
    std::vector<gui_element*>::reverse_iterator i=children.rbegin();
    for(;i!=children.rend();i++)
    {
        if(*i!=nullptr)
        {
        ret=(*i)->get_element_from_point(x,y);
        if(ret!=nullptr)
            return ret;
        }
    }
    if(this->is_visible()&&absolute_rect.is_point_inside(x,y))
        ret=this;
    return ret;
}
