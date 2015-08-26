#pragma once
#include "GUIEventListener.h"
#include "utility/Rect2d.h"
#include "utility/Vector.h"

enum GUIElementType
{
    GUIET_ELEMENT,

    GUIET_BUTTON,
    GUIET_CHECKBOX,
    GUIET_EDITBOX,
    GUIET_IMAGE,
    GUIET_PANE,
    GUIET_SLIDER,
    GUIET_STATIC_TEXT,
    GUIET_WINDOW,

    GUIET_COUNT
};
class GUIEnvironment;
class GUIElement:public GUIEventListener
{
public:
    GUIElement(GUIEnvironment* env,Rect2D<int> dimensions);

    ~GUIElement();

    GUIElementType GetElementType(){return this->Type;}

    void DestroyChildren();

    void SetId(uint32_t);

    uint32_t GetId();

    //each element handles it's own rendering
    virtual void Render();

    void RenderChildren();

    void AddChild(GUIElement *e);

    void RemoveChild(GUIElement *e);

    void BringToFront(GUIElement *e);

    void UpdateAbsolutePos();

    void Move(const glm::vec2 &pos);

    void SetName(std::string name);

    void SetEventListener(GUIEventListener *listener);

    GUIEventListener * GetEventListener();

    virtual bool OnEvent(const GUIEvent & e);

    void SetEnabled(bool b);

    void SetFocused(bool b);

    void SetVisible(bool b);

    //!Purpose: Debug
    void SetHovered(bool b);

    void SetListening(bool b);

    bool IsEnabled();

    bool IsFocused();

    bool IsVisible();

    bool IsHovered();

    bool AcceptsEvents();

    GUIEnvironment *GetEnvironment();

    void SetParent(GUIElement *e);

    GUIElement *GetParent();

    vector<GUIElement*> & GetChildren();

    std::string GetName();

    GUIElement *GetElementFromPoint(int x, int y);

    GUIElement * get_element_by_name(const std::string & name);

    template<class T> T * get_element_by_name_t(const std::string & name)
    {
        return dynamic_cast<T*>(search_elements(this,name));
    }

    Rect2D<int> &GetAbsoluteRect();
    Rect2D<int> &GetRelativeRect();
private:
    GUIElement * search_elements(GUIElement * el, const std::string & name);
protected:
    GUIElementType Type;
    uint32_t Id;
    int disp_w, disp_h;
    std::string name;
    GUIElement *parent;
    vector<GUIElement*> children;
    GUIEnvironment *environment;
    GUIEventListener *event_listener;
    Rect2D<int> absolute_rect, relative_rect;
    bool visible, enabled, accept_events, focused, hovered;
};
