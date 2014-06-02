#include "precomp.h"

#include "gui/gui_environment.h"
#include "gui/gui_static_text.h"
#include "gui/font_renderer.h"
#include "gui/gui_button.h"
#include "gui/gui_window.h"
#include "gui/gui_image.h"
#include "gui/gui_pane.h"
#include "gui/gui_edit_box.h"
#include "gui/toggle_group.h"

#include "resources/mesh_loader.h"
#include "resources/shader_loader.h"
#include "resources/image_loader.h"
#include "scenegraph/sg_graphics_manager.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/isg_render_queue.h"
#include "scenegraph/sg_scenegraph_loader.h"
#include "scenegraph/sg_objects.h"
#include "scenegraph/sg_material.h"
#include "scenegraph/sg_skybox_object.h"

#include "menu_state.h"
#include "wait_for_connection_state.h"

menu_state::menu_state(state_manager* sm):game_state(sm)
{
    m_env=m_app_ctx->env;
    quads=new toggle_group();
    levels=new toggle_group();
    m_rot=0;
}

#define init_e(x) x->set_event_listener(this); elems.push_back(x)
void menu_state::on_load()
{
    img=new gui_image(m_env,rect2d<int>(768,0,462,256),m_app_ctx->gm->load_texture("res/logo_quad3.png"));
    init_e(img);

    btn=new gui_button(m_env,rect2d<int>(100,300,200,40),L"Pradėti",true,false);
    btn->set_name("start");
    init_e(btn);

    btn=new gui_button(m_env,rect2d<int>(100,360,200,40),L"Laiko išbandymas",true,false);
    btn->set_name("timetrial");
    init_e(btn);
    btn->set_enabled(false);

    btn=new gui_button(m_env,rect2d<int>(100,420,200,40),L"Nustatymai",true,false);
    btn->set_name("options");
    init_e(btn);
    btn->set_enabled(false);

    btn=new gui_button(m_env,rect2d<int>(100,480,200,40),L"Išeiti");
    btn->set_name("exit");
    init_e(btn);

    create_start_window();

    quads->set_toggle(0);
    levels->set_toggle(0);

    create_scene();
}

void menu_state::create_scene()
{
    sg::sg_camera_object_ptr cam=sg::sg_camera_object_ptr(new sg::sg_camera_object(m_app_ctx->sg,glm::vec3(0,5,-5),glm::vec3(0,0,0),glm::vec3(0,1,0),1.777777f,45.0f,1.0f,10000.f));
    m_app_ctx->sg->set_active_camera(cam);

    ///load model
    mesh_ptr m=m_app_ctx->gm->get_mesh_loader()->load("res/quadcopters/unshielded_fast_quad.iqm");
    if(!m) exit(-1);

    obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_app_ctx->sg,m));
    obj->set_position(glm::vec3(0,0,0));

    sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
    sm_mat->mat_texture= m_app_ctx->gm->load_texture("res/quadcopters/red.png");

    m_app_ctx->sg->add_object(obj);

    sg::sg_light_object_ptr light = m_app_ctx->sg->add_light_object();
    light->set_position(glm::vec3(0,100,0));

    sg::sg_skybox_object_ptr skybox = m_app_ctx->sg->load_skybox("res/models/sky1/sky.iqm",true);
    m_app_ctx->sg->add_object(skybox);
}



void menu_state::create_quad_selections()
{
    pane=new gui_pane(m_env,rect2d<int>(10,50,748,220),true);
    pane->set_name("quad_panel");
    pane->set_parent(m_env->get_element_by_name("win_normal"));
    init_e(pane);

    st=new gui_static_text(m_env,rect2d<int>(10,10,200,20),L"Orlaivio tipas:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(st);


    btn=new gui_button(m_env,rect2d<int>(10,30,80,80),L"",true,false);
    btn->set_name("QS1");
    btn->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(btn);
    quads->add(btn);

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/light.png"));
    btn->set_image(img);

    st=new gui_static_text(m_env,rect2d<int>(10,110,50,20),L"Paprastasis",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(100,30,80,80),L"",true,false);
    btn->set_name("QS2");
    btn->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(btn);
    quads->add(btn);

    st=new gui_static_text(m_env,rect2d<int>(100,110,50,20),L"Be apsaugos",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(190,30,80,80),L"",true,false);
    btn->set_name("QS3");
    btn->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(btn);
    quads->add(btn);

    st=new gui_static_text(m_env,rect2d<int>(190,110,50,20),L"Mini",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(280,30,80,80),L"",true,false);
    btn->set_name("QS4");
    btn->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(btn);
    quads->add(btn);
    btn->set_enabled(false);

    st=new gui_static_text(m_env,rect2d<int>(280,110,50,20),L"Greitasis",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(370,30,80,80),L"",true,false);
    btn->set_name("QS5");
    btn->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(btn);
    quads->add(btn);
    btn->set_enabled(false);

    st=new gui_static_text(m_env,rect2d<int>(370,110,50,20),L"Sunkusis",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(st);

    pane=new gui_pane(m_env,rect2d<int>(5,125,450,90),true);
    pane->set_name("quad_info_panel");
    pane->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(pane);

    st=new gui_static_text(m_env,rect2d<int>(10,10,200,20),L"Orlaivio informacija:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_info_panel"));
    init_e(st);

    st=new gui_static_text(m_env,rect2d<int>(10,30,100,20),L"Sudėtingumas:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_info_panel"));
    init_e(st);

    st=new gui_static_text(m_env,rect2d<int>(10,50,100,20),L"Greitis:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_info_panel"));
    init_e(st);

    st=new gui_static_text(m_env,rect2d<int>(10,70,100,20),L"Mąsė:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_info_panel"));
    init_e(st);


    pane=new gui_pane(m_env,rect2d<int>(500,10,200,200),true);
    pane->set_name("quad_preview_panel");
    pane->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(pane);

    st=new gui_static_text(m_env,rect2d<int>(5,5,200,20),L"Orlaivio išvaizda:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_preview_panel"));
    init_e(st);
}

void menu_state::create_level_selections()
{
    pane=new gui_pane(m_env,rect2d<int>(10,270,748,220),true);
    pane->set_name("level_panel");
    pane->set_parent(m_env->get_element_by_name("win_normal"));
    init_e(pane);

    st=new gui_static_text(m_env,rect2d<int>(10,10,200,20),L"Lygio Pasirinkimas:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);


    btn=new gui_button(m_env,rect2d<int>(10,30,80,80),L"",true,false);
    btn->set_name("LS1");
    btn->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(btn);
    levels->add(btn);

    st=new gui_static_text(m_env,rect2d<int>(10,110,50,20),L"Treniruotė 1",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(100,30,80,80),L"",true,false);
    btn->set_name("LS2");
    btn->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(btn);
    levels->add(btn);

    st=new gui_static_text(m_env,rect2d<int>(100,110,50,20),L"Treniruotė 2",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(190,30,80,80),L"",true,false);
    btn->set_name("LS3");
    btn->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(btn);
    levels->add(btn);
    btn->set_enabled(false);

    st=new gui_static_text(m_env,rect2d<int>(190,110,50,20),L"Lygis 1",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(280,30,80,80),L"",true,false);
    btn->set_name("LS4");
    btn->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(btn);
    levels->add(btn);
    btn->set_enabled(false);

    st=new gui_static_text(m_env,rect2d<int>(280,110,50,20),L"Lygis 2",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(370,30,80,80),L"",true,false);
    btn->set_name("LS5");
    btn->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(btn);
    levels->add(btn);
    btn->set_enabled(false);

    st=new gui_static_text(m_env,rect2d<int>(370,110,50,20),L"Lygis 3",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    pane=new gui_pane(m_env,rect2d<int>(5,125,450,90),true);
    pane->set_name("level_info_panel");
    pane->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(pane);

    st=new gui_static_text(m_env,rect2d<int>(10,10,200,20),L"Lygio informacija:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_info_panel"));
    init_e(st);
}

void menu_state::create_start_window()
{
    win=new gui_window(m_env,rect2d<int>(400,100,768,540),L"Pasirinkite orlaivį ir lygį",true,false,false,false);
    win->set_name("win_normal");
    init_e(win);

    st=new gui_static_text(m_env,rect2d<int>(15,30,50,20),L"Vartotojas:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("win_normal"));
    init_e(st);

    eb=new gui_edit_box(m_env,rect2d<int>(80,25,100,20),m_app_ctx->gd->user,glm::vec4(0,0,0,1),false,true,false);
    eb->set_parent(m_env->get_element_by_name("win_normal"));
    init_e(eb);

    create_quad_selections();
    create_level_selections();

    btn=new gui_button(m_env,rect2d<int>(344,495,80,30),L"Pradėti");
    btn->set_name("start_level");
    btn->set_parent(m_env->get_element_by_name("win_normal"));
    init_e(btn);

    win->set_visible(false);
}

void menu_state::on_unload()
{
    for(gui_element* el:elems)
    {
        m_env->remove_child(el);
    }
    m_app_ctx->sg->clear();
    //m_app_ctx->se->stopAllSounds();
}

void menu_state::start()
{
    m_app_ctx->se->play2D("../../res/sounds/main.ogg",true,false);
}

void menu_state::update(float delta)
{
    m_rot+=delta*10;
    m_env->update(delta);
    glm::quat q=glm::quat();
    q=glm::rotate(q,m_rot,glm::vec3(0,1,0));
    obj->set_rotation(q);
    m_app_ctx->sg->update_all(delta);
}

bool menu_state::on_event(const gui_event &e)
{
    switch(e.get_type())
    {
    case gui_event_type::element_hovered:
        if(e.get_caller()->get_element_type()==GUIET_button)
            m_app_ctx->se->play2D("../../res/sounds/gui_select.ogg");
        break;
    case button_pressed:
        if(e.get_caller()->get_element_type()==GUIET_button)
            m_app_ctx->se->play2D("../../res/sounds/gui_click.ogg");
        break;
    case button_released:
        if(e.get_caller()->get_name().find("QS")!=std::string::npos)
        {
            quads->set_toggle(atoi(e.get_caller()->get_name().substr(2).c_str())-1);
        }
        if(e.get_caller()->get_name().find("LS")!=std::string::npos)
        {
            levels->set_toggle(atoi(e.get_caller()->get_name().substr(2).c_str())-1);
        }
        if(e.get_caller()->get_name()=="start")
        {
            gui_element* elem=m_env->get_element_by_name("win_normal");
            elem->set_visible(!elem->is_visible());
            //m_state_manager->set_state(new default_game_state(m_state_manager,L"\"Yep, it worked.\" - Abraham Lincoln regarding that almost dead nigga."));
            return true;
        }

        if(e.get_caller()->get_name()=="start_level")
        {
            gui_element* elem=m_env->get_element_by_name("win_normal");
            elem->set_visible(!elem->is_visible());
            m_state_manager->set_state(new wait_for_connection_state(m_state_manager));
            return true;
        }

        if(e.get_caller()->get_name()=="exit")
        {
            m_state_manager->exit();
            //m_state_manager->set_state(new default_game_state(m_state_manager,L"\"Yep, it worked.\" - Abraham Lincoln regarding that almost dead nigga."));
            return true;
        }
        break;
    case window_closed:
        if(e.get_caller()->get_name()=="win_normal")
        {
            gui_button* elem=(gui_button*)m_env->get_element_by_name("start");
            elem->set_toggled(!elem->is_toggled());
            //m_state_manager->set_state(new default_game_state(m_state_manager,L"\"Yep, it worked.\" - Abraham Lincoln regarding that almost dead nigga."));
            return true;
        }
        break;
    default:
        break;
    }
    return false;
}

void menu_state::render()
{
    glClearColor(0.2,0.2,0.4,1);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    m_app_ctx->sg->render_all();
    m_env->render();
}

