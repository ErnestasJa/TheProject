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
#include "gui/gui_window.h"

test_kursinis::test_kursinis(uint32_t argc, const char ** argv): application(argc,argv)
{
    simuliuoti = false;
}

test_kursinis::~test_kursinis()
{

}

bool test_kursinis::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);
    wnd->sig_key_event().connect(sigc::mem_fun(this,&test_kursinis::on_key_event));
    wnd->sig_window_resized().connect(sigc::mem_fun(this,&test_kursinis::on_resize));

    m_scenegraph = new sg::scenegraph(this->get_logger(),this->get_timer());
    m_graphics_manager = m_scenegraph->get_graphics_manager();

    ///gl setup
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    if(!init_scene())
        return false;

    main_timer->tick();

    gui_skin s=gui_skin();
    s.load("../../res/skin_default.xml");

    env=new gui_environment(this->wnd,this->get_logger());

    gui_window * wnd = new gui_window(env,rect2d<int>(10,10,200,400));

    gui_button* btn=new gui_button(env,rect2d<int>(10,0,64,64),"HOLA");
    btn->set_name("hola_btn");
    btn->set_event_listener(this);
    btn->set_parent(wnd);


    gui_checkbox* cb=new gui_checkbox(env,rect2d<int>(0,64,20,20),false);
    cb->set_parent(wnd);
    cb=new gui_checkbox(env,rect2d<int>(10,96,20,20),true);
    cb->set_parent(wnd);
    cb=new gui_checkbox(env,rect2d<int>(10,128,20,20),true);
    cb->set_parent(wnd);
    cb=new gui_checkbox(env,rect2d<int>(10,160,20,20),false);
    cb->set_parent(wnd);

    gui_edit_box* eb=new gui_edit_box(env,rect2d<int>(10,10,180,20),"",glm::vec4(1,1,1,1),false,false);
    eb->set_parent(wnd);

    glm::vec2 aaa=env->get_font_renderer()->get_text_dimensions("bybys raibas");
    printf("Dimensions of bybys raibas: %f %f\n",aaa.x,aaa.y);

    return true;
}

float saules_mase = 1988550.0;
float zemes_mase = 5.972f;
float menulio_mase = 0.07346f;

bool test_kursinis::init_scene()
{
    uint32_t w = wnd->get_window_size().x, h = wnd->get_window_size().y;
    glViewport(0,0,w,h);


    ///obj0
    ///saule
    {
        obj[0] = new Objektas(m_scenegraph->load_mesh_object("res/test_kursinis/Sphere.iqm",false),saules_mase,glm::vec3(0.0,0.0,0.0));
        m_scenegraph->add_object(obj[0]->get_object());
        obj[0]->get_object()->set_position(glm::vec3(0.0,0.0,0.0));
    }

    ///obj1
    ///zeme
    {
        obj[1] = new Objektas(m_scenegraph->load_mesh_object("res/test_kursinis/Sphere.iqm",false),zemes_mase,glm::vec3(-4.0,0.0,0.0));
        m_scenegraph->add_object(obj[1]->get_object());
        obj[1]->get_object()->set_position(glm::vec3(0.0,0.0,-150.0));
    }

    ///obj2
    ///menulis
    {
        obj[2] = new Objektas(m_scenegraph->load_mesh_object("res/test_kursinis/Sphere.iqm",false),menulio_mase,glm::vec3(-4.0,0.0,0.0));
        m_scenegraph->add_object(obj[2]->get_object());
        obj[2]->get_object()->set_position(glm::vec3(0.0,0.0,-155.0));
    }



    m_scenegraph->add_object(sg::sg_camera_object_ptr(new sg::sg_camera_object(m_scenegraph,glm::vec3(1,600,1),glm::vec3(0,0,0),glm::vec3(0,1,0))));
    auto light = m_scenegraph->add_light_object();
    light->set_position(glm::vec3(0,10,0));

    /*sg::sg_scenegraph_loader sg_loader;
    sg_loader.load_scene(m_scenegraph,"res/test_kursinis/test.ibs");*/

    return true;
}

static const float dt = 1.0/10;

bool test_kursinis::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        main_timer->tick();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        if(simuliuoti)
        {
            glm::vec3 f0 = Objektas::calc_force(obj[0],obj[1]) + Objektas::calc_force(obj[0],obj[2]);
            glm::vec3 f1 = Objektas::calc_force(obj[1],obj[0]) + Objektas::calc_force(obj[1],obj[2]);
            glm::vec3 f2 = Objektas::calc_force(obj[2],obj[0]) + Objektas::calc_force(obj[2],obj[1]);

            ///0
            obj[0]->set_force(f0);
            glm::vec3 dv0 = f0*dt;
            glm::vec3 v0 = obj[0]->get_velocity();
            obj[0]->set_velocity(v0 + dv0);
            glm::vec3 dp0 = v0*dt;

            ///1
            obj[1]->set_force(f1);
            glm::vec3 dv1 = f1*dt;
            glm::vec3 v1 = obj[1]->get_velocity();
            obj[1]->set_velocity(v1 + dv1);
            glm::vec3 dp1 = v1*dt;

            ///2
            obj[2]->set_force(f2);
            glm::vec3 dv2 = f2*dt;
            glm::vec3 v2 = obj[2]->get_velocity();
            obj[2]->set_velocity(v2 + dv2);
            glm::vec3 dp2 = v2*dt;

            obj[0]->get_object()->set_position(obj[0]->get_object()->get_position()+dp0);
            obj[1]->get_object()->set_position(obj[1]->get_object()->get_position()+dp1);
            obj[2]->get_object()->set_position(obj[2]->get_object()->get_position()+dp2);
        }

        m_scenegraph->update_all(0.01);
        m_scenegraph->render_all();

        env->update(0);
        env->render();

        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;
}

void test_kursinis::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{
        if(sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera())
    {
        if(action==GLFW_PRESS || action == GLFW_REPEAT )
        {
            if(key==GLFW_KEY_W)
                cam->walk(1);
            if(key==GLFW_KEY_S)
                cam->walk(-1);
            if(key==GLFW_KEY_A)
                cam->strafe(-1);
            if(key==GLFW_KEY_D)
                cam->strafe(1);
        }

        if(action==GLFW_PRESS)
        {
            if(key==GLFW_KEY_SPACE)
            {
                glm::vec3 pos   = cam->get_position();
                glm::vec3 look  = glm::normalize(cam->get_look());

                ///load model
                mesh_ptr m=m_graphics_manager->get_mesh_loader()->load("res/trashcan.iqm");
                if(!m) return;

                sg::sg_mesh_object_ptr obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_scenegraph,m));
                obj->set_position(pos+look*10.0f);

                sg::sg_material_static_mesh * sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
                sm_mat->mat_texture= m_graphics_manager->load_texture("res/no_tex.png");

                m_scenegraph->add_object(obj);
            }

        }
    }
}

void test_kursinis::on_event(gui_event e)
{


    switch(e.get_type())
    {
        case button_pressed:
        {
            m_log->log(LOG_LOG, "Got event type: %i; button_pressed: %i.",e.get_type() , button_pressed);
            m_log->log(LOG_LOG, "Element name: '%s'.", e.get_caller()->get_name().c_str());

            if( e.get_caller()->get_name() == "hola_btn" )
            {
                simuliuoti = !simuliuoti;
                m_log->log(LOG_LOG, "Simuliuoti: '%i'.", (int)simuliuoti);
            }
        }
    }
}

void test_kursinis::on_resize(int32_t w, int32_t h)
{
    glViewport(0,0,w,h);
}

void test_kursinis::exit()
{
    loopi(3)
        delete obj[i];
    delete env;
    application::exit();
}
