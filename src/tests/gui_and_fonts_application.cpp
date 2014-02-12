#include "precomp.h"

#include "application/window.h"
#include "utility/timer.h"
#include "utility/logger.h"
#include "opengl/opengl_util.h"

#include "gui/gui_environment.h"
#include "gui/gui_static_text.h"
#include "gui/gui_button.h"

#include "gui_and_fonts_application.h"

#include "math/rect2d.h"

gui_and_fonts_application::gui_and_fonts_application(uint32_t argc, const char ** argv): application(argc,argv)
{
    //definitely a comment
}

gui_and_fonts_application::~gui_and_fonts_application()
{

}

bool gui_and_fonts_application::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);

    frame_count = 0;
    last_time = 0;

    ///set up matrices

    M = glm::mat4(1.0f);
    M = glm::translate(M,glm::vec3(-10,5,-10));
    M = glm::scale(M,glm::vec3(0.01,0.01,0.01));

    V = glm::lookAt(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0));
    V = glm::rotate<float>(V,-90,glm::vec3(0,1,0));

    P = glm::perspective(45.f,4.f/3.f,1.0f,2048.f);

    MVP=P*V*M;

    ///gl setup
    glClearColor(0.0f, 0.0f, 0.568f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    int ww,hh;
    GLFWwindow * _window=wnd->getWindow();
    glfwGetWindowSize(_window,&ww,&hh);

    env=new gui_environment(ww,hh,_window);
    test1=new gui_static_text(env,rect2d<int>(900,10,125,12),"testtext",glm::vec4(0,0,0,1),true);
    env->add_child(test1);
    test2=new gui_button(env,rect2d<int>(200,100,100,50),"buttons!");
    test2->set_event_listener(this);

    renderer=env->get_font_renderer();
    renderer->create_font("bits","res/bits.ttf",28);
    return true;
}

void gui_and_fonts_application::on_event(gui_event e)
{
    logger* _log=this->get_logger();
    switch(e.get_type())
    {
    case element_focused:
        _log->log(LOG_DEBUG,"Element %s got focused.",e.get_caller()->get_name().c_str());
        break;
    case element_focus_lost:
        _log->log(LOG_DEBUG,"Element %s lost focus.",e.get_caller()->get_name().c_str());
        break;
    case element_hovered:
        _log->log(LOG_DEBUG,"Element %s got hovered.",e.get_caller()->get_name().c_str());
        break;
    case element_exitted:
        _log->log(LOG_DEBUG,"Element %s got exitted.",e.get_caller()->get_name().c_str());
        break;

    case button_pressed:
        _log->log(LOG_DEBUG,"Button pressed on %s.",e.get_caller()->get_name().c_str());
        break;

    case button_released:
        _log->log(LOG_DEBUG,"Button released on %s.",e.get_caller()->get_name().c_str());
        break;

    case mouse_pressed:
        _log->log(LOG_DEBUG,"Mouse pressed on %s.",e.get_caller()->get_name().c_str());
        break;
    case mouse_released:
        _log->log(LOG_DEBUG,"Mouse released on %s.",e.get_caller()->get_name().c_str());
        break;
    case mouse_dragged:
        _log->log(LOG_DEBUG,"Mouse dragged on %s.",e.get_caller()->get_name().c_str());
        break;
    case mouse_moved:
        _log->log(LOG_DEBUG,"Mouse moved on %s.",e.get_caller()->get_name().c_str());
        break;
    default:
        break;
    }
}

bool gui_and_fonts_application::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        main_timer->tick();

        env->update(0);

        show_fps();

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        env->render();

        renderer->render_string("TCHOFF tchoff PHYSFS!",glm::vec2(0,10));
        renderer->render_string("bits","I am le lcd machine!",glm::vec2(0,100),glm::vec4(1,1,1,1));

        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;
}

void gui_and_fonts_application::show_fps()
{
    uint32_t currentTime = main_timer->get_time();
    frame_count++;
    if ( currentTime - last_time >= 1000 )  // If last prinf() was more than 1 sec ago
    {
        m_log->log(LOG_LOG,"FPS: %i (%f ms/frame)",frame_count,1000.0/double(frame_count));

        test1->set_text("FPS: "+helpers::to_str<int>(frame_count)+" "+helpers::to_str(1000.0/double(frame_count))+"ms");

        frame_count = 0;
        last_time = currentTime;
    }
}

void gui_and_fonts_application::exit()
{
    delete env;
    application::exit();
}
