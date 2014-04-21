#include "precomp.h"

#include "application/window.h"
#include "utility/timer.h"
#include "utility/logger.h"
#include "opengl/opengl_util.h"
#include "opengl/texture.h"
#include "resources/image_loader.h"

#include "test_kursinis.h"

#include "scenegraph/sg_graphics_manager.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/isg_render_queue.h"
#include "scenegraph/sg_scenegraph_loader.h"
#include "scenegraph/sg_objects.h"
#include "scenegraph/sg_material.h"

#include "math/rect2d.h"
#include "gui/gui_skin.h"
#include "gui/gui_environment.h"
#include "gui/gui_button.h"
#include "gui/gui_checkbox.h"
#include "gui/gui_pane.h"
#include "gui/gui_edit_box.h"
#include "gui/gui_image.h"

test_kursinis::test_kursinis(uint32_t argc, const char ** argv): application(argc,argv)
{
}

test_kursinis::~test_kursinis()
{

}

bool test_kursinis::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);
    wnd->sig_key_event().connect(sigc::mem_fun(this,&test_kursinis::on_key_event));

    m_scenegraph = new sg::scenegraph(this->get_logger(),this->get_timer());
    m_graphics_manager = m_scenegraph->get_graphics_manager();

    ///gl setup
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);

    if(!init_scene())
        return false;

    main_timer->tick();

    /*gui_skin s=gui_skin();
    s.load("../../res/skin_default.xml");

    env=new gui_environment(this->wnd,this->get_logger());

    gui_button* btn=new gui_button(env,rect2d<int>(0,0,64,64),"HOLA");

    gui_checkbox* cb=new gui_checkbox(env,rect2d<int>(0,64,20,20),false);
    cb=new gui_checkbox(env,rect2d<int>(0,96,20,20),true);
    cb=new gui_checkbox(env,rect2d<int>(0,128,20,20),true);
    cb=new gui_checkbox(env,rect2d<int>(0,160,20,20),false);

    gui_edit_box* eb=new gui_edit_box(env,rect2d<int>(200,0,200,20),"",glm::vec4(1,1,1,1),false,false);

    glm::vec2 aaa=env->get_font_renderer()->get_text_dimensions("bybys raibas");
    printf("Dimensions of bybys raibas: %f %f\n",aaa.x,aaa.y);
*/
    return true;
}

bool test_kursinis::init_scene()
{
    uint32_t w = wnd->get_window_size().x, h = wnd->get_window_size().y;
    glViewport(0,0,w,h);

    auto obj = m_scenegraph->load_mesh_object("res/test_kursinis/ico_sphere.iqm",true);

    if(!obj)
        throw obj;

    obj = m_scenegraph->load_mesh_object("res/trashcan.iqm",true);

    if(!obj)
        throw obj;

    m_scenegraph->add_object(obj);
    m_scenegraph->add_object(sg::sg_camera_object_ptr(new sg::sg_camera_object(m_scenegraph,glm::vec3(0,10,0),glm::vec3(0,0,0),glm::vec3(0,1,0))));
    auto light = m_scenegraph->add_light_object();
    light->set_position(glm::vec3(0,-10,0));

    return true;
}

bool test_kursinis::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        main_timer->tick();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


        m_scenegraph->update_all(0.01);
        m_scenegraph->render_all();

        //env->update(0);
        //env->render();

        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;
}

void test_kursinis::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{

}

void test_kursinis::exit()
{
    //delete env;
    application::exit();
}
