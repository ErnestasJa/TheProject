#include "precomp.h"

#include "application/window.h"
#include "utility/timer.h"
#include "utility/logger.h"
#include "opengl/opengl_util.h"
#include "opengl/texture.h"
#include "resources/image_loader.h"

#include "kursinis.h"

#include "scenegraph/sg_graphics_manager.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/isg_render_queue.h"
#include "scenegraph/sg_scenegraph_loader.h"
#include "scenegraph/sg_objects.h"
#include "scenegraph/sg_material.h"
#include "scenegraph/sg_line_object.h"

#include "math/rect2d.h"
#include "gui/gui_skin.h"
#include "gui/gui_environment.h"
#include "gui/gui_button.h"
#include "gui/gui_static_text.h"
#include "gui/gui_checkbox.h"
#include "gui/gui_pane.h"
#include "gui/gui_edit_box.h"
#include "gui/gui_image.h"
#include "gui/gui_window.h"
#include "gui/gui_slider.h"


float angle_x=5, angle_y=0, distance=600;

kursinis::kursinis(uint32_t argc, const char ** argv): application(argc,argv)
{
    window_closed = false;
    simuliuoti = false;
    fixed_time_step = 1000.0/60.0;
    gravitational_constant = 6.67;
    sub_steps = 1;
}

kursinis::~kursinis()
{

}

bool kursinis::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);
    wnd->sig_key_event().connect(sigc::mem_fun(this,&kursinis::on_key_event));
    wnd->sig_mouse_key().connect(sigc::mem_fun(this,&kursinis::on_mouse_key_event));
    wnd->sig_mouse_moved().connect(sigc::mem_fun(this,&kursinis::on_mouse_move));
    wnd->sig_window_resized().connect(sigc::mem_fun(this,&kursinis::on_resize));

    m_scenegraph = new sg::scenegraph(this->get_logger(),this->get_timer());
    m_graphics_manager = m_scenegraph->get_graphics_manager();

    ///gl setup
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    if(!init_scene())
        return false;

    main_timer->tick();


    return init_gui(width, height);
}


bool kursinis::init_gui(uint32_t width, uint32_t height)
{
    gui_skin s=gui_skin();
    s.load("../../res/skin_default.xml");

    env=new gui_environment(this->wnd,this->get_logger());
    env->set_event_listener(this);

    uint32_t main_wnd_width = 220;
    gui_window * wnd = main_wnd = new gui_window(env,rect2d<int>(10,10,main_wnd_width,230),L"");

    #define init_e(x) x->set_parent(wnd); //x->set_event_listener(this)
    gui_button* btn = new gui_button(env,rect2d<int>(5,140,main_wnd_width-10,20),L"Pradėti");
    btn->set_name("sim_btn");
    init_e(btn);

    btn = new gui_button(env,rect2d<int>(5,165,main_wnd_width-10,20),L"Objektų param.");
    btn->set_name("pos_btn");
    init_e(btn);

    btn = new gui_button(env,rect2d<int>(5,190,main_wnd_width-10,20),L"Išvalyti trajektorijas");
    btn->set_name("clear_trajectories");
    init_e(btn);


    #define ADD_SLIDER(text,name,y,min,max)\
    {gui_static_text * txt = new gui_static_text(env,rect2d<int>(10,(y)+5,130,20),(text));\
    init_e(txt);\
    gui_slider * slider = new gui_slider(env,rect2d<int>(140,(y),64,20),(min),(max),0);\
    slider->set_name((name));\
    init_e(slider);}

    ADD_SLIDER(L"Kameros nuotolis:","cam_dist_slider",30,30,2000)

    gui_static_text *tb = new gui_static_text(env,rect2d<int>(10,85,130,20),L"Požingsnių kiekis:");
    sub_step_eb=new gui_edit_box(env,rect2d<int>(140,80,64,20),L"",glm::vec4(1,1,1,1),false,false);
    sub_step_eb->set_name("sub_step_eb");
    init_e(sub_step_eb);
    init_e(tb);

    tb = new gui_static_text(env,rect2d<int>(10,65,130,20),L"Laiko žingsnis:");
    fixed_time_step_eb=new gui_edit_box(env,rect2d<int>(140,60,64,20),L"",glm::vec4(1,1,1,1),false,false);
    fixed_time_step_eb->set_name("fixed_time_step_eb");
    init_e(fixed_time_step_eb);
    init_e(tb);

    tb = new gui_static_text(env,rect2d<int>(10,110,130,20),L"Gravitacinė konstanta:");
    grav_constant_eb=new gui_edit_box(env,rect2d<int>(140,105,64,20),L"",glm::vec4(1,1,1,1),false,false);
    grav_constant_eb->set_name("grav_constant_eb");
    init_e(grav_constant_eb);
    init_e(tb);



    ///init wnd
    wnd = pos_wnd = new gui_window(env,rect2d<int>(10,240,270,400),L"Objektų parametrų langas");
    pos_wnd->set_visible(false);

    int32_t start_h = 24;
    loopi(3)
    {
        gui_static_text * tb = new gui_static_text(env,rect2d<int>(10,start_h+5
                                                                    + (i * 32),48,20),L"");
        tb->set_parent(wnd);

        pos_eb[i][0]=new gui_edit_box(env,rect2d<int>(110     ,  start_h + (i * 32), 48,20),L"",glm::vec4(1,1,1,1),false,false);
        pos_eb[i][1]=new gui_edit_box(env,rect2d<int>(110 + 50,  start_h + (i * 32), 48,20),L"",glm::vec4(1,1,1,1),false,false);
        pos_eb[i][2]=new gui_edit_box(env,rect2d<int>(110 + 100, start_h + (i * 32), 48,20),L"",glm::vec4(1,1,1,1),false,false);

        init_e(pos_eb[i][0]);
        init_e(pos_eb[i][1]);
        init_e(pos_eb[i][2]);

        switch(i)
        {
        case 0:
            tb->set_text(L"Pozicija_1(x,y,z)");
            break;
        case 1:
            tb->set_text(L"Pozicija_2(x,y,z)");
            break;
        case 2:
            tb->set_text(L"Pozicija_3(x,y,z)");
            break;
        }
    }



    start_h = 128;

    loopi(3)
    {
        gui_static_text * tb = new gui_static_text(env,rect2d<int>(10, start_h+5 + (i * 32),48,20),L"");
        tb->set_parent(wnd);

        vel_eb[i][0]=new gui_edit_box(env,rect2d<int>(110     ,  start_h + (i * 32), 48,20),L"",glm::vec4(1,1,1,1),false,false);
        vel_eb[i][1]=new gui_edit_box(env,rect2d<int>(110 + 50,  start_h + (i * 32), 48,20),L"",glm::vec4(1,1,1,1),false,false);
        vel_eb[i][2]=new gui_edit_box(env,rect2d<int>(110 + 100, start_h + (i * 32), 48,20),L"",glm::vec4(1,1,1,1),false,false);

        init_e(vel_eb[i][0]);
        init_e(vel_eb[i][1]);
        init_e(vel_eb[i][2]);

        switch(i)
        {
        case 0:
            tb->set_text(L"Greitis_1(x,y,z)");
            break;
        case 1:
            tb->set_text(L"Greitis_2(x,y,z)");
            break;
        case 2:
            tb->set_text(L"Greitis_3(x,y,z)");
            break;
        }
    }

    start_h = 224;

    ///masiu ivedimas
    tb = new gui_static_text(env,rect2d<int>(10,start_h+5,40,20),L"Masė_1");
    tb->set_parent(wnd);
    eb[0]=new gui_edit_box(env,rect2d<int>(110,start_h,120,20),L"",glm::vec4(1,1,1,1),false,false);

    tb = new gui_static_text(env,rect2d<int>(10,start_h+5+30,40,20),L"Masė_2");
    tb->set_parent(wnd);
    eb[1]=new gui_edit_box(env,rect2d<int>(110,start_h+30,120,20),L"",glm::vec4(1,1,1,1),false,false);

    tb = new gui_static_text(env,rect2d<int>(10,start_h+5+60,40,20),L"Masė_3");
    tb->set_parent(wnd);
    eb[2]=new gui_edit_box(env,rect2d<int>(110,start_h+60,120,20),L"",glm::vec4(1,1,1,1),false,false);

    init_e(eb[0]);
    init_e(eb[1]);
    init_e(eb[2]);

    btn = new gui_button(env,rect2d<int>(164,370,84,20),L"Atnaujinti");
    btn->set_name("refresh_btn");
    init_e(btn);

    btn = new gui_button(env,rect2d<int>(84,370,84,20),L"Nustatyti");
    btn->set_name("set_btn");
    init_e(btn);

    msg_wnd = new gui_window(env,rect2d<int>(500,360,180,60),L"Pranešimas");

    tb = new gui_static_text(env,rect2d<int>(20,25,40,20),L"Įvyko kūnų susidūrimas.");
    tb->set_parent(msg_wnd);
    tb = new gui_static_text(env,rect2d<int>(20,45,40,20),L"Skaičiavimai sustabdyti.");
    tb->set_parent(msg_wnd);

    msg_wnd->set_visible(false);


    update_ui();
    update_ui_init();

    return true;
}

#include <sstream>
void kursinis::update_ui()
{
    std::wstringstream ss;

    ss<<fixed_time_step;
    fixed_time_step_eb->set_text(ss.str().c_str());
    ss.str(std::wstring());

    ss<<gravitational_constant;
    grav_constant_eb->set_text(ss.str().c_str());
    ss.str(std::wstring());

    ss<<sub_steps;
    sub_step_eb->set_text(ss.str().c_str());
    ss.str(std::wstring());


    gui_slider * s = this->env->get_element_by_name_t<gui_slider>("cam_dist_slider");

    if(s)
    {
        s->set_value(distance);
        this->get_logger()->log(LOG_LOG, "SLIDER value = %f; cam y = %f",s->get_value(),distance);
    }
}

void kursinis::update_ui_init()
{
    loopi(3)
    {
        Objektas * o = obj[i];
        const glm::dvec3 & vel = o->get_velocity();
        const glm::dvec3 & pos = o->get_position();

        std::wstringstream ss;

        ///pos
        ss<<pos.x;
        pos_eb[i][0]->set_text(ss.str());
        ss.str(std::wstring());

        ss<<pos.y;
        pos_eb[i][1]->set_text(ss.str());
        ss.str(std::wstring());

        ss<<pos.z;
        pos_eb[i][2]->set_text(ss.str());
        ss.str(std::wstring());


        ///vel
        ss<<vel.x;
        vel_eb[i][0]->set_text(ss.str());
        ss.str(std::wstring());

        ss<<vel.y;
        vel_eb[i][1]->set_text(ss.str());
        ss.str(std::wstring());

        ss<<vel.z;
        vel_eb[i][2]->set_text(ss.str());
        ss.str(std::wstring());

        ss << o->get_mass();
        eb[i]->set_text(ss.str());
        ss.str(std::wstring());
    }
}

void kursinis::set_object_values_from_ui()
{
    double v = 0;

    loopi(3)
    {
        Objektas * o = obj[i];
        glm::dvec3 vel;
        glm::dvec3 pos;
        double mass;

        std::wstringstream ss;

        ///pos
        ss<<pos_eb[i][0]->get_text();
        if(ss>>v) pos.x = v;
        ss.str(std::wstring());
        ss.clear();

        ss<<pos_eb[i][1]->get_text();
        if(ss>>v) pos.y = v;
        ss.str(std::wstring());
        ss.clear();

        ss<<pos_eb[i][2]->get_text();
        if(ss>>v) pos.z = v;
        ss.str(std::wstring());
        ss.clear();

        ///vel
        ss<<vel_eb[i][0]->get_text();
        if(ss>>v) vel.x = v;
        ss.str(std::wstring());
        ss.clear();

        ss<<vel_eb[i][1]->get_text();
        if(ss>>v) vel.y = v;
        ss.str(std::wstring());
        ss.clear();

        ss<<vel_eb[i][2]->get_text();
        if(ss>>v) vel.z = v;
        ss.str(std::wstring());
        ss.clear();

        ss<<eb[i]->get_text();
        ss>>mass;
        ss.str(std::wstring());
        ss.clear();

        o->set_position(pos);
        o->set_velocity(vel);
        o->set_mass(mass);
        o->update_graphics_object();
    }
}

void kursinis::set_simulation_values_from_ui()
{
    double fts=0.0;
    double sbs=0;
    std::wstringstream ss;

    ss<<sub_step_eb->get_text();

    if(ss>>sbs)
    {
        ss.str(std::wstring());
        ss.clear();
        sub_steps = (uint32_t)sbs;
        ss<<sub_steps;
        sub_step_eb->set_text(ss.str());
    }
    ss.str(std::wstring());
    ss.clear();

    ss<<this->fixed_time_step_eb->get_text();
    if(ss>>fts)
        fixed_time_step=fts;

    ss.str(std::wstring());
    ss.clear();

    ss<<this->grav_constant_eb->get_text();
    if(ss>>fts)
        gravitational_constant=fts;

    ss.str(std::wstring());
    ss.clear();

}

void init_obj(Objektas * obj, sg::sg_graphics_manager_ptr gmgr, glm::vec3 color, glm::vec3 pos)
{
    auto m = gmgr->create_material(sg::SGM_ABSTRACT_MATERIAL,"res/shaders/kursinis/color_mat.vert","res/shaders/kursinis/color_mat.frag");
    sg::sg_abstract_material* mat = static_cast<sg::sg_abstract_material*>(m.get());
    mat->set_vec3f("color",color);

    obj->get_object()->set_material(0,m);
    obj->get_object()->set_position(pos);

    mat = obj->get_line_object()->get_abstract_material();
    obj->get_line_object()->add_segment(obj->get_object()->get_position(),obj->get_object()->get_position());
    mat->set_vec3f("color",color);
}

bool kursinis::init_scene()
{
    uint32_t w = wnd->get_window_size().x, h = wnd->get_window_size().y;
    glViewport(0,0,w,h);


    ///obj0
    ///saule
    {
        obj[0] = new Objektas(m_scenegraph->load_mesh_object("res/test_kursinis/Sphere.iqm",false), sg::sg_line_object_ptr(new sg::sg_line_object(m_scenegraph)),
                              1,glm::dvec3(4.0,0.0,0.0),glm::dvec3(0.0,0.0,-4.0));
        m_scenegraph->add_object(obj[0]->get_object());
        m_scenegraph->add_object(obj[0]->get_line_object());
        init_obj(obj[0],this->m_graphics_manager, glm::vec3(1,0,0), glm::vec3(0,0,-4));
    }

    ///obj1
    ///zeme
    {
        obj[1] = new Objektas(m_scenegraph->load_mesh_object("res/test_kursinis/Sphere.iqm",false), sg::sg_line_object_ptr(new sg::sg_line_object(m_scenegraph)),
                              1,glm::dvec3(0.0,0.0,0.0),glm::dvec3(0.0,0.0,0.0));
        m_scenegraph->add_object(obj[1]->get_object());
        m_scenegraph->add_object(obj[1]->get_line_object());
        init_obj(obj[1],this->m_graphics_manager, glm::vec3(0,1,0), glm::vec3(0,0,0));
    }

    ///obj2
    ///menulis
    {
        obj[2] = new Objektas(m_scenegraph->load_mesh_object("res/test_kursinis/Sphere.iqm",false), sg::sg_line_object_ptr(new sg::sg_line_object(m_scenegraph)),
                              1,glm::dvec3(-4.0,0.0,0.0),glm::dvec3(0.0,0.0,4.0));
        m_scenegraph->add_object(obj[2]->get_object());
        m_scenegraph->add_object(obj[2]->get_line_object());
        init_obj(obj[2],this->m_graphics_manager, glm::vec3(0,0,1), glm::vec3(0,0,4));
    }



    m_scenegraph->add_object(sg::sg_camera_object_ptr(new sg::sg_camera_object(m_scenegraph,glm::vec3(0,600,5),glm::vec3(0,0,0),glm::vec3(0,1,0))));
    auto light = m_scenegraph->add_light_object();
    light->set_position(glm::vec3(0,10,0));

    sg::sg_skybox_object_ptr skybox = m_scenegraph->load_skybox("res/models/sky2/sky.iqm",true);
    m_scenegraph->add_object(skybox);


    /*sg::sg_scenegraph_loader sg_loader;
    sg_loader.load_scene(m_scenegraph,"res/test_kursinis/test.ibs");*/

    return true;
}

bool kursinis::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        main_timer->tick();
        last_time = current_time;
        current_time = main_timer->get_time();
        double delta_time = ((double)(current_time - last_time))*0.001;

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);


        if(simuliuoti)
        {
            double sub_step_time = ((delta_time * fixed_time_step) / sub_steps);

            glm::dvec3 _pos[3];

            loopi(3)
                _pos[i] = obj[i]->get_position();

            loopi(sub_steps)
            {
                    if(Objektas::check_collision(obj[0],obj[1])||Objektas::check_collision(obj[0],obj[2])||Objektas::check_collision(obj[2],obj[1]))
                    {
                        msg_wnd->set_visible(true);
                        env->bring_to_front(msg_wnd);

                        simuliuoti = false;
                        ((gui_button*)env->get_element_by_name("sim_btn"))->set_text(L"Pradėti");

                        break;
                    }

                    glm::dvec3 a0 = Objektas::calc_accel(obj[0],obj[1],gravitational_constant) + Objektas::calc_accel(obj[0],obj[2],gravitational_constant, &Objektas::calc_accel_newton);
                    glm::dvec3 a1 = Objektas::calc_accel(obj[1],obj[0],gravitational_constant) + Objektas::calc_accel(obj[1],obj[2],gravitational_constant, &Objektas::calc_accel_newton);
                    glm::dvec3 a2 = Objektas::calc_accel(obj[2],obj[0],gravitational_constant) + Objektas::calc_accel(obj[2],obj[1],gravitational_constant, &Objektas::calc_accel_newton);

                    ///0
                    glm::dvec3 dv0 = a0*sub_step_time;
                    glm::dvec3 v0 = obj[0]->get_velocity();
                    obj[0]->set_velocity(v0 + dv0);
                    glm::dvec3 dp0 = v0*sub_step_time;

                    ///1
                    glm::dvec3 dv1 = a1*sub_step_time;
                    glm::dvec3 v1 = obj[1]->get_velocity();
                    obj[1]->set_velocity(v1 + dv1);
                    glm::dvec3 dp1 = v1*sub_step_time;

                    ///2
                    glm::dvec3 dv2 = a2*sub_step_time;
                    glm::dvec3 v2 = obj[2]->get_velocity();
                    obj[2]->set_velocity(v2 + dv2);
                    glm::dvec3 dp2 = v2*sub_step_time;

                    obj[0]->set_position(obj[0]->get_position()+dp0);
                    obj[1]->set_position(obj[1]->get_position()+dp1);
                    obj[2]->set_position(obj[2]->get_position()+dp2);
            }

            if(simuliuoti)
            loopi(3)
            {
                obj[i]->update_graphics_object();
                obj[i]->get_line_object()->add_segment(glm::vec3(_pos[i]),obj[i]->get_object()->get_position());
                obj[i]->get_line_object()->update_mesh();
            }

        }


        m_scenegraph->update_all(0.01);
        m_scenegraph->render_all();

        env->update(0);
        env->render();

        wnd->swap_buffers();

        sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera();
        cam->orbit(glm::vec3(0,0,0),distance,angle_x,angle_y);


        if(window_closed)
            return false;

        return !this->gl_util->check_and_output_errors();
    }
    return false;
}

void kursinis::on_mouse_key_event(int32_t button, int32_t action, int32_t mod)
{
    if(button==GLFW_MOUSE_BUTTON_1&&action==GLFW_PRESS)
    {
        int32_t x = this->wnd->get_mouse_pos().x, y = this->wnd->get_mouse_pos().y;
        int32_t w = this->wnd->get_window_size().x, h = this->wnd->get_window_size().y;
        sg::sg_object_ptr o = this->m_scenegraph->object_depth_pick(x,y,w,h);

        if(o)
        {
            glm::vec3 v = o->get_position();
            this->m_log->log(LOG_LOG, "Object '%s' pos: (%f, %f, %f);",o->get_name().c_str() ,v.x ,v.y ,v.z);

            if(!simuliuoti)
            {
                selected_obj = o;
                mouse_start = this->wnd->get_mouse_pos();
                obj_start = v;
            }
        }
        else
            selected_obj = sg::sg_object_ptr();


    }
    else if(button==GLFW_MOUSE_BUTTON_1&&action==GLFW_RELEASE)
    {
        selected_obj = sg::sg_object_ptr();
    }
}

void kursinis::on_mouse_move(double x, double y)
{
    glm::vec2 mouse_delta = glm::vec2(x,y)-mouse_start;

    if(selected_obj)
    {
        selected_obj->set_position(selected_obj->get_position()+glm::vec3(mouse_delta.x,0,mouse_delta.y));
    }
}

void kursinis::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{
        if(sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera())
    {
        if(action==GLFW_PRESS || action == GLFW_REPEAT )
        {
            if(key==GLFW_KEY_W)
                angle_x += 0.5;
            if(key==GLFW_KEY_S)
                angle_x -= 0.5;
            if(key==GLFW_KEY_A)
                angle_y += 0.5;
            if(key==GLFW_KEY_D)
                angle_y -= 0.5;
        }

        if(action==GLFW_PRESS)
        {
            if(key==GLFW_KEY_F1)
            {
                main_wnd->set_visible(!main_wnd->is_visible());
            }
        }
    }
}

bool kursinis::on_event(const gui_event & e)
{
    switch(e.get_type())
    {
        case button_pressed:
        {
            m_log->log(LOG_LOG, "Got event type: %i\nElement name: '%s'.", e.get_type(), e.get_element()->get_name().c_str());

            if( e.get_element()->get_name() == "pos_btn" )
            {
                pos_wnd->set_visible(!pos_wnd->is_visible());

                return true;
            }
            else if( e.get_element()->get_name() == "sim_btn" )
            {
                simuliuoti = !simuliuoti;

                if(simuliuoti)
                    selected_obj = nullptr;

                gui_button * btn = static_cast< gui_button *>(e.get_element());
                btn->set_text(simuliuoti?L"Sustabdyti":L"Pradėti");

                m_log->log(LOG_LOG, "Pradėti: '%i'.", (int)simuliuoti);
                return true;
            }
            else if( e.get_element()->get_name() == "refresh_btn" )
            {
                update_ui_init();

                return true;
            }
            else if( e.get_element()->get_name() == "set_btn" )
            {
                set_object_values_from_ui();

                return true;
            }
            else if( e.get_element()->get_name() == "clear_trajectories" )
            {
                loopi(3)
                    obj[i]->get_line_object()->clear_segments();

                return true;
            }
            break;
        }
        case scrollbar_changed:
        {
            if(e.get_element()->get_name()=="cam_dist_slider")
            {
                gui_slider * s = static_cast<gui_slider *>(e.get_element());
                distance = s->get_value();
            }
            else if(e.get_element()->get_name()=="sub_step_slider")
            {
                gui_slider * s = static_cast<gui_slider *>(e.get_element());
                sub_steps = s->get_value();
            }

            break;
        }
        case element_focus_lost:
        {
            if(e.get_element()->get_name()=="fixed_time_step_eb")
            {
                 std::cout<<"eb lost focus...\n";
                 set_simulation_values_from_ui();
                 //return true;
            }
            else if(e.get_element()->get_name()=="grav_constant_eb")
            {
                 std::cout<<"eb lost focus...\n";
                 set_simulation_values_from_ui();
                 //return true;
            }
            else if(e.get_element()->get_name()=="sub_step_eb")
            {
                 std::cout<<"eb lost focus...\n";
                 set_simulation_values_from_ui();
                 //return true;
            }

            break;
        }
        default:
            break;
    }
    return false;
}

void kursinis::on_resize(int32_t w, int32_t h)
{
    glViewport(0,0,w,h);
}

void kursinis::on_window_close()
{
    window_closed = true;
}

void kursinis::exit()
{
    loopi(3)
        delete obj[i];
    delete env;
    application::exit();
}
