#pragma once

// Stuff to still convert
//!TODO (murloc992#1#): GUI Event
//!TODO (murloc992#1#): GUI Event Listener
//!TODO (murloc992#1#): Rectangle
//!TODO (murloc992#1#): Vec2
//!TODO (murloc992#1#): GUI Element
//!TODO (murloc992#1#): GUI Environment
//!TODO (murloc992#1#): Font Renderer
//!TODO (murloc992#1#): GUI Static Text
//!TODO (murloc992#1#): GUI Button
//!TODO (murloc992#1#): GUI Window
//TODO (murloc992#1#): GUI Text Field
//TODO (murloc992#1#): GUI Slider

#include "gui_element.h"
#include "gui_skin.h"
#include "font_renderer.h"

class shader;
class quad;
class sliced_gui_quad;
class texture;
class window;
class gui_skin;
class logger;
class gui_environment : public gui_element
{
public:
    gui_environment(window* win, logger* log);
    ~gui_environment();

    void update(float delta);
    void render();

    virtual bool on_event(const gui_event & e);
    bool is_on_hover(gui_element *e);
    bool is_on_focus(gui_element *e);

    void draw_gui_quad(rect2d<int> size, std::shared_ptr<texture> tex, bool tile=false);
    void draw_gui_quad(rect2d<int> size, uint32_t style=gui_style::gui_skin_background, bool tile=false);
    void draw_sliced_gui_quad(rect2d<int> size, std::shared_ptr<texture> tex, bool tile=false);
    void draw_sliced_gui_quad(rect2d<int> size, uint32_t style=gui_style::gui_skin_background, bool tile=false);

    void set_skin(gui_skin* skin);

    glm::vec2 get_mouse_pos();
    glm::vec2 get_gui_scale();

    void on_mouse_moved(double x, double y);
    void on_mouse_button(int32_t button, int32_t action, int32_t mod);
    void on_mouse_scroll(double sx, double sy);
    void on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t mod);
    void on_char_typed(int32_t scan_code);

    const std::string &get_clipboard()
    {
        return clipboard_string;
    }

    char get_last_char()
    {
        return last_char;
    }

    int32_t get_last_key()
    {
        return last_key;
    }

    template <typename T>
    rect2d<T> scale_gui_rect(rect2d<T> unscaled)
    {
        T gsx=get_gui_scale().x;
        T gsy=get_gui_scale().y;
        T px=-1+unscaled.x*gsx+unscaled.w/2*gsx;
        T py=1-unscaled.y*gsy-unscaled.h/2*gsy;
        T sx=unscaled.w/2*gsx;
        T sy=unscaled.h/2*gsy;

        rect2d<T> ret=rect2d<T>(px,py,sx,sy);

        return ret;
    }

    font_renderer *get_font_renderer();


    ///GUI ELEMENTS
//    gui_static_text *add_gui_static_text();
//    gui_button *add_gui_button();
//    gui_checkbox *add_gui_checkbox();
//    gui_edit_box *add_gui_edit_box();
//
//    gui_window *add_gui_window();
//    gui_pane* add_gui_pane();
private:
    gui_skin* skin;
    texture* skin_atlas;
    shader* gui_shader;

    quad* gui_quad;
    sliced_gui_quad* sliced_quad;

    font_renderer* m_font_renderer;
    window* m_window;

    sigc::connection _sig_mouse_move,_sig_mouse_button,_sig_mouse_scroll,_sig_key,_sig_text;

    gui_element *hover, *last_hover, *focus, *last_focus;

    bool m_mouse_down, m_mouse_moved, m_mouse_dragged;

    char last_char;
    int32_t last_key,last_mod;
    std::string clipboard_string;

    glm::vec2 mouse_pos, last_mouse_pos, gui_scale;
protected:
};
