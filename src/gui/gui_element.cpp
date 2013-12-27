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
    set_name("gui_element_"+helpers::to_str(rand()%65535));
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

    e->relative_rect=e->absolute_rect;

    update_absolute_pos();
}

void gui_element::remove_child(gui_element *e)
{
    for(std::vector<gui_element*>::iterator i=children.begin(); i!=children.end(); i++)
        if((gui_element*)(&i)==e)
        {
            e->parent=nullptr;
            children.erase(i);
            (*i)->relative_rect=(*i)->absolute_rect;
            return;
        }
}

void gui_element::bring_to_front(gui_element *e)
{
    if(children.size()>0)
    {
        for(std::vector<gui_element*>::iterator i=children.begin(); i!=children.end(); i++)
        {

            if(*i == e)
            {
                i=children.erase(i);
                children.push_back(e);
                return;
            }
            else
                i++;
        }
    }
}

void gui_element::render_children()
{
    for(gui_element* i : children)
    {
        if(i->is_visible())
            i->render();
    }
}

void gui_element::update_absolute_pos()
{
    if(this->parent!=nullptr)
        this->absolute_rect=
            rect2d<int>(parent->absolute_rect.x+relative_rect.x,
                            parent->absolute_rect.y+relative_rect.y,
                            this->absolute_rect.w, this->absolute_rect.h);
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
    if(e!=nullptr)
    {
        if(this->parent!=nullptr)
            this->parent->remove_child(this);
        e->add_child(this);
    }
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

rect2d<int> &gui_element::get_absolute_rect()
{
    return absolute_rect;
}

rect2d<int> &gui_element::get_relative_rect()
{
    return relative_rect;
}
