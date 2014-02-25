#include "precomp.h"

#include "gui_environment.h"
#include "opengl/shader.h"
#include "opengl/quad.h"
#include "opengl/sliced_gui_quad.h"
#include "gui_skin.h"
#include "resources/image.h"
#include "resources/image_loader.h"
#include "opengl/texture.h"
#include "utility/logger.h"
#include "application/window.h"

gui_environment::gui_environment(window* win)
{
    win->sig_mouse_moved().connect(sigc::mem_fun(this,&gui_environment::on_mouse_moved);
    win->sig_mouse_button().connect(sigc::mem_fun(this,&gui_environment::on_mouse_button);
    win->sig_mouse_scroll().connect(sigc::mem_fun(this,&gui_environment::on_mouse_scroll);
    win->sig_key_event().connect(sigc::mem_fun(this,&gui_environment::on_key_event);

    this->m_window=win;

    glm::ivec2 win_dims=m_window->get_window_size();
    this->disp_w=win_dims.x;
    this->disp_h=win_dims.y;

    gui_element(nullptr, rect2d<int>(0,0,disp_w,disp_h));

    hover=last_hover=focus=last_focus=nullptr;

    m_mouse_down=m_mouse_dragged=m_mouse_moved=false;

    mouse_pos=last_mouse_pos=glm::vec2();

    gui_scale=glm::vec2(2.0/(float)dispw,2.0/(float)disph);

    this->set_name("GUI_ENVIRONMENT");
    last_char=' ';

    gui_shader=shader::load_shader("res/gui_quad");
    gui_quad=new quad();
    gui_quad->generate();

    sliced_quad=new sliced_gui_quad();
    sliced_quad->generate();

    skin=new gui_skin();

    skin->load("../../res/skin_default.xml");

    skin_atlas=new texture();
    image_loader* imgl=new image_loader(log);
    std::shared_ptr<image> img=std::shared_ptr<image>(imgl->load("res/skin_default.png"));
    skin_atlas->init(img);

    m_font_renderer=new font_renderer(this);
}

gui_environment::~gui_environment()
{
    delete m_font_renderer;
    delete gui_shader;
    delete gui_quad;
}

void gui_environment::update(float delta)
{
    //hovering
    glm::vec2 tm=input->get_mouse_pos();
    mouse_pos=input->get_mouse_pos();

    gui_element *target = get_element_from_point(tm.x, tm.y);

    //only update elements which are enabled,visible and accept events
    if (target != nullptr)
        if (target->is_enabled() && target->is_visible() && target->accepts_events())
        {

            if (target != hover && target != nullptr && m_mouse_dragged == false)
            {
                if (hover != nullptr)
                {
                    last_hover = hover;
                    last_hover->on_event(gui_event(
                                             gui_event_type::element_exitted,
                                             last_hover));
                                             last_hover->set_hovered(false);
                }
                hover = target;
                hover->on_event(gui_event(
                                    gui_event_type::element_hovered, hover));
                                    hover->set_hovered(true);
            }

            //focusing
            if (input->mouse_button(0) && hover != focus
                    && m_mouse_dragged == false)
            {
                if (focus != nullptr)
                {
                    last_focus = focus;
                    last_focus->on_event(gui_event(
                                             gui_event_type::element_focus_lost,
                                             last_focus));
                                             last_focus->set_focused(false);
                }
                focus = hover;
                if (hover != this)
                {
                    focus->on_event(gui_event(
                                        gui_event_type::element_focused, focus));
                                        focus->set_focused(true);
                    focus->get_parent()->bring_to_front(focus);
                }
                else
                    focus = nullptr;
            }

            if (input->mouse_button(0) && focus != nullptr
                    && m_mouse_down == false)
            {
                focus->on_event(gui_event(
                                    gui_event_type::mouse_pressed, this));
                m_mouse_down = true;
            }

            if (!input->mouse_button(0) && m_mouse_down && focus != nullptr)
            {
                focus->on_event(gui_event(
                                    gui_event_type::mouse_released, this));
                m_mouse_down = false;
            }

            if (mouse_pos != last_mouse_pos)
            {
                m_mouse_moved = true;
                last_mouse_pos=mouse_pos;
                //printf("mouse has moved..\n");
                if (input->mouse_button(0))
                    m_mouse_dragged = true;
                else
                    m_mouse_dragged = false;
            }
            else
                m_mouse_moved = false;

            if (m_mouse_moved && focus != nullptr && !mouse_dragged)
                focus->on_event(gui_event(
                                    gui_event_type::mouse_moved, this));

            if (m_mouse_down && m_mouse_moved && focus != nullptr)
                focus->on_event(gui_event(
                                    gui_event_type::mouse_dragged, this));
        }
}

void gui_environment::render()
{
    this->render_children();
}

void gui_environment::on_event(gui_event e)
{
    switch(e.get_type())
    {
    case element_focused:
        this->bring_to_front(e.get_caller());
        break;
    default:
        break;
    }
}

void gui_environment::on_key_pressed(int32_t key, int32_t scan_code, int32_t action, int32_t mod)
{
    this->last_char=scan_code;
    if(focus!=nullptr)
        focus->on_event(gui_event(key_typed,this));
}

glm::vec2 gui_environment::get_mouse_pos()
{
    return this->input->get_mouse_pos();
}

glm::vec2 gui_environment::get_gui_scale()
{
    return gui_scale;
}

font_renderer* gui_environment::get_font_renderer()
{
    return m_font_renderer;
}

void gui_environment::draw_gui_quad(rect2d<int> dims,uint32_t style,bool tile)
{
    rect2d<float> scaled_dims=scale_gui_rect(dims.as<float>());

    glEnable(GL_BLEND);

    gui_shader->set();
    skin_atlas->set(0);

    gui_quad->set_uv(skin->get_uv(style));

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));

    gui_quad->draw();

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,0);
}


void gui_environment::draw_sliced_gui_quad(rect2d<int> dims,uint32_t style,bool tile)
{
    rect2d<float> scaled_dims=scale_gui_rect(dims.as<float>());

    glEnable(GL_BLEND);

    gui_shader->set();
    skin_atlas->set(0);

    //sliced_quad->set_tcoords(skin->get_uv(gui_skin_input_active));

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));

    sliced_quad->draw();

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,0);
}
