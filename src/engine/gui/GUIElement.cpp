#include "Precomp.h"

#include "gui/GUIElement.h"

GUIElement::GUIElement(GUIEnvironment* env,Rect2D<int> dimensions)
{
    this->Type=GUIET_ELEMENT;
    absolute_rect = dimensions;
    relative_rect = dimensions;

    Id=-1;

    hovered = false;
    visible = true;
    focused = false;
    enabled = true;
    accept_events = true;

    this->parent = nullptr;
    this->event_listener = nullptr;
    this->environment = env;
    SetName("gui_element_"+helpers::to_str(rand()%65535));
}

GUIElement::~GUIElement()
{
    DestroyChildren();
}

void GUIElement::SetId(uint32_t Id)
{
    this->Id=Id;
}

uint32_t GUIElement::GetId()
{
    return this->Id;
}

void GUIElement::DestroyChildren()
{
    for(GUIElement* el:children)
    {
        el->SetListening(false);
    }

    children.clear();
}

void GUIElement::Render()
{
    //will be overriden by everything
}

void GUIElement::AddChild(GUIElement *e)
{
    vector<GUIElement*>::iterator i=std::find(children.begin(),children.end(),e);
    if(i!=children.end())
        return;

    e->parent=this;
    children.push_back(e);

    e->relative_rect=e->absolute_rect;

    UpdateAbsolutePos();
}

void GUIElement::RemoveChild(GUIElement *e)
{
    vector<GUIElement*>::iterator i=std::find(children.begin(),children.end(),e);
    if(i!=children.end())
    {
        e->parent=nullptr;
        i=children.erase(i);
        (*i)->relative_rect=(*i)->absolute_rect;
        return;
    }
}

void GUIElement::BringToFront(GUIElement *e)
{
    vector<GUIElement*>::iterator i=std::find(children.begin(),children.end(),e);
    if(i!=children.end())
    {
        i=children.erase(i);
        children.push_back(e);
        return;
    }
}

void GUIElement::RenderChildren()
{
    for(GUIElement* i : children)
    {
        if(i->IsVisible())
            i->Render();
    }
}

void GUIElement::UpdateAbsolutePos()
{
    if(this->parent!=nullptr)
        this->absolute_rect=
            Rect2D<int>(parent->absolute_rect.x+relative_rect.x,
                        parent->absolute_rect.y+relative_rect.y,
                        this->absolute_rect.w, this->absolute_rect.h);
    for(GUIElement *e : children)
        e->UpdateAbsolutePos();
}

void GUIElement::Move(const glm::vec2 &pos)
{
    relative_rect.SetPosition(pos.x,pos.y);
    UpdateAbsolutePos();
}

void GUIElement::SetName(std::string name)
{
    this->name=name;
}

void GUIElement::SetEventListener(GUIEventListener *listener)
{
    this->event_listener=listener;
}

GUIEventListener * GUIElement::GetEventListener()
{
    return this->event_listener;
}

bool GUIElement::OnEvent(const GUIEvent & e)
{
    if(this->event_listener && this->event_listener->OnEvent(e))
        return true;

    return parent ? parent->OnEvent(e) : false;
}

void GUIElement::SetEnabled(bool b)
{
    this->enabled=b;
    if(children.size()>0)
        for(GUIElement *e : children)
            e->SetEnabled(true);
}

void GUIElement::SetFocused(bool b)
{
    this->focused=b;
}

void GUIElement::SetVisible(bool b)
{
    this->visible=b;
    for(GUIElement* e:children)
        e->SetVisible(b);
}

void GUIElement::SetHovered(bool b)
{
    this->hovered=b;
}

void GUIElement::SetListening(bool b)
{
    this->accept_events=b;
}

void GUIElement::SetParent(GUIElement *e)
{
    if(e!=nullptr)
    {
        if(this->parent!=nullptr)
            this->parent->RemoveChild(this);
        e->AddChild(this);
    }
    else
        return;
}

bool GUIElement::IsEnabled()
{
    return this->enabled;
}

bool GUIElement::IsFocused()
{
    return this->focused;
}

bool GUIElement::IsVisible()
{
    return this->visible;
}

bool GUIElement::IsHovered()
{
    return this->hovered;
}

bool GUIElement::AcceptsEvents()
{
    return this->accept_events;
}

GUIEnvironment *GUIElement::GetEnvironment()
{
    return this->environment;
}

GUIElement *GUIElement::GetParent()
{
    return this->parent;
}

vector<GUIElement *> & GUIElement::GetChildren()
{
    return this->children;
}

std::string GUIElement::GetName()
{
    return this->name;
}

GUIElement *GUIElement::GetElementFromPoint(int x, int y)
{
    GUIElement *ret=nullptr;
    vector<GUIElement*>::reverse_iterator i=children.rbegin();
    for(; i!=children.rend(); i++)
    {
        if(*i!=nullptr)
        {
            ret=(*i)->GetElementFromPoint(x,y);
            if(ret!=nullptr&&ret->AcceptsEvents())
                return ret;
        }
    }
    if(this->IsVisible()&&absolute_rect.is_point_inside(x,y))
        ret=this;
    return ret;
}

Rect2D<int> &GUIElement::GetAbsoluteRect()
{
    return absolute_rect;
}

Rect2D<int> &GUIElement::GetRelativeRect()
{
    return relative_rect;
}

GUIElement * GUIElement::search_elements(GUIElement * el, const std::string & name)
{
    if(el->GetName()==name)
    {
        return el;
    }

    loop(i,el->GetChildren().size())
    {
        GUIElement * e = search_elements(el->GetChildren()[i],name);

        if(e)
        {
            return e;
        }
    }

    return nullptr;
}

GUIElement * GUIElement::get_element_by_name(const std::string & name)
{
    return search_elements(this,name);
}
