#include "precomp.h"

#include "gui_environment.h"
#include "opengl/shader.h"
#include "opengl/quad.h"
#include "opengl/sliced_gui_quad.h"
#include "gui_skin.h"
#include "resources/image.h"
#include "resources/image_loader.h"
#include "opengl/texture.h"
#include "application/window.h"

gui_environment::gui_environment(window* win,logger* log):gui_element(nullptr, rect2d<int>(0,0,win->get_window_size().x,win->get_window_size().y))
{
    _sig_mouse_move=win->sig_mouse_moved().connect(sigc::mem_fun(this,&gui_environment::on_mouse_moved));
    _sig_mouse_button=win->sig_mouse_key().connect(sigc::mem_fun(this,&gui_environment::on_mouse_button));
    _sig_mouse_scroll=win->sig_mouse_scroll().connect(sigc::mem_fun(this,&gui_environment::on_mouse_scroll));
    _sig_key=win->sig_key_event().connect(sigc::mem_fun(this,&gui_environment::on_key_event));
    _sig_text=win->sig_text_event().connect(sigc::mem_fun(this,&gui_environment::on_char_typed));

    this->m_window=win;

    glm::ivec2 win_dims=m_window->get_window_size();
    this->disp_w=win_dims.x;
    this->disp_h=win_dims.y;

    hover=last_hover=focus=last_focus=nullptr;

    m_mouse_down=m_mouse_dragged=m_mouse_moved=false;

    mouse_pos=last_mouse_pos=glm::vec2();

    gui_scale=glm::vec2(2.0/(float)disp_w,2.0/(float)disp_h);

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
    _sig_mouse_move.disconnect();
    _sig_mouse_button.disconnect();
    _sig_mouse_scroll.disconnect();
    _sig_key.disconnect();
    _sig_text.disconnect();
    delete m_font_renderer;
    delete gui_shader;
    delete gui_quad;
}

void gui_environment::update(float delta)
{
}

void gui_environment::render()
{
    this->render_children();
}

bool gui_environment::on_event(const gui_event & e)
{
    GUI_BEGIN_ON_EVENT(e)

    switch(e.get_type())
    {
    case element_focused:
        this->bring_to_front(e.get_caller());
        break;
    default:
        break;
    }

    GUI_END_ON_EVENT(e)
}

void gui_environment::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t mod)
{
    //printf("Key event: Key:%i SC:%i Action:%i Mod:%i\n",key,scan_code,action,mod);

    this->last_key=key;

    if(focus!=nullptr)
    {
        switch(action)
        {
        case GLFW_PRESS:
            switch(mod)
            {
            case GLFW_MOD_CONTROL:
                switch(key)
                {
                case GLFW_KEY_A:
                    break;
                case GLFW_KEY_X:
                    break;
                case GLFW_KEY_C:
                    break;
                case GLFW_KEY_V:
                    this->clipboard_string=helpers::to_wstr(glfwGetClipboardString(this->m_window->getWindow()));
                    focus->on_event(gui_event(text_paste,focus));
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            focus->on_event(gui_event(key_pressed,focus));
            break;
        case GLFW_REPEAT:
            focus->on_event(gui_event(key_pressed,focus));
            break;
        case GLFW_RELEASE:
            break;

        default:
            break;
        }
    }
}

void gui_environment::on_char_typed(int32_t scan_code)
{
    this->last_char=scan_code;
    if(focus!=nullptr)
        focus->on_event(gui_event(key_typed,focus));
}

void gui_environment::on_mouse_moved(double x, double y)
{
    mouse_pos=glm::vec2(x,y);

    gui_element *target = get_element_from_point(mouse_pos.x, mouse_pos.y);

    //only update elements which are enabled,visible and accept events
    if (target != nullptr)
        if (target->is_enabled() && target->is_visible() && target->accepts_events())
        {
            if (target != hover)
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

            if(focus!=nullptr)
                if(m_mouse_down)
                    focus->on_event(gui_event(mouse_dragged,focus));
        }


}

void gui_environment::on_mouse_button(int32_t button, int32_t action, int32_t mod)
{
    switch(button)
    {
    case GLFW_MOUSE_BUTTON_LEFT:
        switch(action)
        {
        case GLFW_PRESS:
            m_mouse_down=true;

            //focusing
            if (hover != focus)
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

            if(focus!=nullptr&&hover==focus)
                focus->on_event(gui_event(mouse_pressed,focus));
            break;
        case GLFW_RELEASE:
            m_mouse_down=false;
            if(focus!=nullptr&&hover==focus)
                focus->on_event(gui_event(mouse_released,focus));
            break;
        default:
            break;
        }
        break;
    case GLFW_MOUSE_BUTTON_RIGHT:
        break;
    case GLFW_MOUSE_BUTTON_MIDDLE:
        break;
    default:
        break;
    }
}

void gui_environment::on_mouse_scroll(double sx, double sy)
{
    return;
}

glm::vec2 gui_environment::get_mouse_pos()
{
    return mouse_pos;
}

glm::vec2 gui_environment::get_gui_scale()
{
    return gui_scale;
}

font_renderer* gui_environment::get_font_renderer()
{
    return m_font_renderer;
}

void gui_environment::draw_gui_quad(rect2d<int> dims,std::shared_ptr<texture> tex,bool tile)
{
    rect2d<float> scaled_dims=scale_gui_rect(dims.as<float>());

    glEnable(GL_BLEND);

    gui_shader->set();
    tex->set(0);

    gui_quad->set_uv(skin->get_uv(gui_skin_whole_texture));

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));

    gui_quad->draw();

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,0);
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


void gui_environment::draw_sliced_gui_quad(rect2d<int> dims,std::shared_ptr<texture> tex,bool tile)
{
    rect2d<float> scaled_dims=scale_gui_rect(dims.as<float>());

    glEnable(GL_BLEND);

    gui_shader->set();
    tex->set(0);

    //sliced_quad->set_tcoords(skin->get_uv(gui_skin_background));

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));

    sliced_quad->draw();

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,0);
}

void gui_environment::draw_sliced_gui_quad(rect2d<int> dims,uint32_t style,bool tile)
{
    rect2d<float> scaled_dims=scale_gui_rect(dims.as<float>());

    glEnable(GL_BLEND);

    gui_shader->set();
    skin_atlas->set(0);

    sliced_quad->set_tcoords(skin->get_uv(gui_skin_background));

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));

    sliced_quad->draw();

    glDisable(GL_BLEND);
    glBindTexture(GL_TEXTURE_2D,0);
}
