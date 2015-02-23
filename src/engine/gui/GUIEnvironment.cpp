#include "Precomp.h"

#include "GUIEnvironment.h"
#include "opengl/Shader.h"
#include "opengl/GUIQuad.h"
#include "opengl/SlicedGUIQuad.h"
#include "GUISkin.h"
#include "resources/Image.h"
#include "resources/ImageLoader.h"
#include "opengl/Texture.h"
#include "application/Window.h"
#include "application/AppContext.h"

GUIEnvironment::GUIEnvironment(AppContext* ctx):GUIElement(nullptr, Rect2D<int>(0,0,ctx->_window->GetWindowSize().x,ctx->_window->GetWindowSize().y))
{
    m_context=ctx;
    ctx->_guiEnv=this;
    this->m_window=ctx->_window;

    _sig_mouse_move=m_window->SigMouseMoved().connect(sigc::mem_fun(this,&GUIEnvironment::on_mouse_moved));
    _sig_mouse_button=m_window->SigMouseKey().connect(sigc::mem_fun(this,&GUIEnvironment::on_mouse_button));
    _sig_mouse_scroll=m_window->SigMouseScroll().connect(sigc::mem_fun(this,&GUIEnvironment::on_mouse_scroll));
    _sig_key=m_window->SigKeyEvent().connect(sigc::mem_fun(this,&GUIEnvironment::on_key_event));
    _sig_text=m_window->SigTextEvent().connect(sigc::mem_fun(this,&GUIEnvironment::on_char_typed));

    glm::ivec2 win_dims=m_window->GetWindowSize();
    this->disp_w=win_dims.x;
    this->disp_h=win_dims.y;

    hover=last_hover=focus=last_focus=nullptr;

    m_mouse_down=m_mouse_dragged=m_mouse_moved=false;

    mouse_pos=last_mouse_pos=glm::vec2();

    gui_scale=glm::vec2(2.0/(float)disp_w,2.0/(float)disp_h);

    this->SetName("GUI_ENVIRONMENT");
    last_char=' ';

    gui_shader=Shader::LoadShader("res/engine/shaders/gui_quad");
    gui_quad=new GUIQuad();
    gui_quad->Init();

    sliced_quad=new SlicedGUIQuad(glm::vec2(1),8);
    sliced_quad->Init();

    skin=new GUISkin();

    skin->load("res/gui/skins/skin_default.xml");

    skin_atlas=new Texture();
    image_loader* imgl=new image_loader(m_context->_logger);
    std::shared_ptr<image> img=std::shared_ptr<image>(imgl->load("res/gui/skins/skin_default2.png"));
    skin_atlas->Init(img);

    delete imgl;

    m_font_renderer=new FontRenderer(ctx);
}

GUIEnvironment::~GUIEnvironment()
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

void GUIEnvironment::update(float delta)
{
}

void GUIEnvironment::Render()
{
    glDisable(GL_DEPTH_TEST);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    this->RenderChildren();
    glDisable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
}

bool GUIEnvironment::OnEvent(const GUIEvent & e)
{
    GUI_BEGIN_ON_EVENT(e)

    switch(e.GetType())
    {
    case element_focused:
        this->BringToFront(e.get_caller());
        break;
    default:
        break;
    }

    GUI_END_ON_EVENT(e)
}

void GUIEnvironment::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t mod)
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
                    this->clipboard_string=helpers::to_wstr(glfwGetClipboardString(this->m_window->GetWindow()));
                    focus->OnEvent(GUIEvent(text_paste,this,focus));
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            focus->OnEvent(GUIEvent(key_pressed,this,focus));
            break;
        case GLFW_REPEAT:
            focus->OnEvent(GUIEvent(key_pressed,this,focus));
            break;
        case GLFW_RELEASE:
            break;

        default:
            break;
        }
    }
}

void GUIEnvironment::on_char_typed(int32_t scan_code)
{
    this->last_char=(wchar_t)scan_code;
    if(focus!=nullptr)
        focus->OnEvent(GUIEvent(key_typed,this,focus));
}

void GUIEnvironment::on_mouse_moved(double x, double y)
{
    mouse_pos=glm::vec2(x,y);

    GUIElement *target = GetElementFromPoint(mouse_pos.x, mouse_pos.y);

    //only update elements which are enabled,visible and accept events
    if (target != nullptr)
        if (target->IsEnabled() && target->IsVisible() && target->AcceptsEvents())
        {
            if (target != hover)
            {
                if (hover != nullptr)
                {
                    last_hover = hover;
                    last_hover->OnEvent(GUIEvent(
                                            gui_event_type::element_exitted,this,
                                            last_hover));
                    last_hover->SetHovered(false);
                }
                hover = target;
                hover->OnEvent(GUIEvent(
                                   gui_event_type::element_hovered,this, hover));
                hover->SetHovered(true);
            }

            if(focus!=nullptr)
                if(m_mouse_down)
                    focus->OnEvent(GUIEvent(mouse_dragged,this,focus));
        }


}

void GUIEnvironment::on_mouse_button(int32_t button, int32_t action, int32_t mod)
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
                    GUI_FIRE_ELEMENT_EVENT(last_focus,GUIEvent(gui_event_type::element_focus_lost,this, last_focus))
                    last_focus->SetFocused(false);
                }
                focus = hover;
                if (hover != this)
                {
                    GUI_FIRE_ELEMENT_EVENT(focus,GUIEvent(gui_event_type::element_focused,this, focus))
                    focus->SetFocused(true);
                    focus->GetParent()->BringToFront(focus);
                }
                else
                    focus = nullptr;
            }

            if(focus!=nullptr&&hover==focus)
                focus->OnEvent(GUIEvent(mouse_pressed,this,focus));
            break;
        case GLFW_RELEASE:
            m_mouse_down=false;
            if(focus!=nullptr&&hover==focus)
                focus->OnEvent(GUIEvent(mouse_released,this,focus));
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

void GUIEnvironment::on_mouse_scroll(double sx, double sy)
{
    return;
}

glm::vec2 GUIEnvironment::get_mouse_pos()
{
    return mouse_pos;
}

glm::vec2 GUIEnvironment::get_gui_scale()
{
    return gui_scale;
}

FontRenderer* GUIEnvironment::get_font_renderer()
{
    return m_font_renderer;
}

void GUIEnvironment::draw_gui_quad(Rect2D<int> dims,std::shared_ptr<Texture> tex,bool tile,bool multichannel)
{
    Rect2D<float> scaled_dims=scale_gui_rect(dims.as<float>());

    gui_shader->Set();
    tex->Set(0);

    gui_quad->SetUV(skin->get_uv(gui_skin_whole_texture));

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));
    glUniform1ui(gui_shader->getparam("coloured"),0);
    if(!multichannel)
        glUniform1ui(gui_shader->getparam("singlechannel"),GL_TRUE);
    else
        glUniform1ui(gui_shader->getparam("singlechannel"),GL_FALSE);
    glUniform1f(gui_shader->getparam("alpha"),1.f);

    gui_quad->Render();

    glBindTexture(GL_TEXTURE_2D,0);
}

void GUIEnvironment::draw_gui_quad(Rect2D<int> dims,uint32_t style,bool tile)
{
    Rect2D<float> scaled_dims=scale_gui_rect(dims.as<float>());

    gui_shader->Set();
    skin_atlas->Set(0);

    gui_quad->SetUV(skin->get_uv(style));

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));
    glUniform1ui(gui_shader->getparam("coloured"),0);

    gui_quad->Render();

    glBindTexture(GL_TEXTURE_2D,0);
}

void GUIEnvironment::draw_gui_quad(Rect2D<int> dims,glm::vec4 col)
{
    Rect2D<float> scaled_dims=scale_gui_rect(dims.as<float>());

    gui_shader->Set();

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));
    glUniform1ui(gui_shader->getparam("coloured"),1);
    glUniform4fv(gui_shader->getparam("color"),1,glm::value_ptr(col));

    gui_quad->Render();
}


void GUIEnvironment::draw_sliced_gui_quad(Rect2D<int> size,std::shared_ptr<Texture> tex,bool tile)
{
    Rect2D<float> scaled_dims=scale_gui_rect(size.as<float>());

    gui_shader->Set();
    tex->Set(0);

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));
    glUniform1ui(gui_shader->getparam("coloured"),0);
    sliced_quad->SetRatio(glm::vec2(size.w,size.h));
    sliced_quad->Render();

    glBindTexture(GL_TEXTURE_2D,0);
}

void GUIEnvironment::draw_sliced_gui_quad(Rect2D<int> size,uint32_t style)
{
    Rect2D<float> scaled_dims=scale_gui_rect(size.as<float>());

    gui_shader->Set();
    skin_atlas->Set(0);

    sliced_quad->SetRatio(glm::vec2(size.w,size.h),skin->get_margin(style));
    sliced_quad->SetTCoords(skin->get_uv(style));

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));
    glUniform1ui(gui_shader->getparam("coloured"),0);
    glUniform1f(gui_shader->getparam("alpha"),0.9f);
    sliced_quad->Render();

    glBindTexture(GL_TEXTURE_2D,0);
}

void GUIEnvironment::draw_sliced_gui_quad(Rect2D<int> size,glm::vec4 col)
{
    Rect2D<float> scaled_dims=scale_gui_rect(size.as<float>());

    gui_shader->Set();

    glm::mat4 M=glm::mat4(1.0f);

    M=glm::translate(M,glm::vec3(scaled_dims.x,scaled_dims.y,0));
    M=glm::scale(M,glm::vec3(scaled_dims.w,scaled_dims.h,0));

    glUniformMatrix4fv(gui_shader->getparam("M"),1,GL_FALSE,glm::value_ptr(M));
    glUniform1ui(gui_shader->getparam("coloured"),1);
    glUniform4fv(gui_shader->getparam("color"),1,glm::value_ptr(col));
    sliced_quad->SetRatio(glm::vec2(size.w,size.h));
    sliced_quad->Render();
}
