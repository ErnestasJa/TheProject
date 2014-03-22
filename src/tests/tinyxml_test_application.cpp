#include "precomp.h"

#include "application/window.h"
#include "utility/timer.h"
#include "utility/logger.h"
#include "opengl/opengl_util.h"

#include "tinyxml_test_application.h"

#include "math/rect2d.h"
#include "gui/gui_skin.h"
#include "gui/gui_environment.h"
#include "gui/gui_button.h"
#include "gui/gui_checkbox.h"
#include "gui/gui_pane.h"
#include "gui/gui_edit_box.h"
#include "gui/gui_window.h"
#include "gui/gui_slider.h"

tinyxml_test_application::tinyxml_test_application(uint32_t argc, const char ** argv): application(argc,argv)
{
}

tinyxml_test_application::~tinyxml_test_application()
{

}

bool tinyxml_test_application::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);

    ///gl setup
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    gui_skin s=gui_skin();
    s.load("../../res/skin_default.xml");

    env=new gui_environment(this->wnd,this->get_logger());

    gui_button* btn=new gui_button(env,rect2d<int>(0,0,64,64),"HOLA");

    gui_checkbox* cb=new gui_checkbox(env,rect2d<int>(0,64,20,20),false);
    cb=new gui_checkbox(env,rect2d<int>(0,96,20,20),true);
    cb=new gui_checkbox(env,rect2d<int>(0,128,20,20),true);
    cb=new gui_checkbox(env,rect2d<int>(0,160,20,20),false);

    gui_edit_box* eb=new gui_edit_box(env,rect2d<int>(200,0,200,20),"",glm::vec4(1,1,1,1),false,false);

    gui_window* testwin= new gui_window(env,rect2d<int>(400,400,200,200),"Testiest window everrrr");
    glm::vec2 aaa=env->get_font_renderer()->get_text_dimensions("bybys raibas");
    printf("Dimensions of bybys raibas: %f %f\n",aaa.x,aaa.y);

    gui_slider* slid=new gui_slider(env,rect2d<int>(0,200,200,20),1,2,1);
    return true;
}

bool tinyxml_test_application::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        main_timer->tick();
        env->update(0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        env->render();
        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;
}

void tinyxml_test_application::exit()
{
    delete env;
    application::exit();
}
