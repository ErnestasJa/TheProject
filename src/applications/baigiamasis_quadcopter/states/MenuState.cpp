#include "Precomp.h"

#include "gui/GUIEnvironment.h"
#include "gui/GUIStaticText.h"
#include "gui/font_rendering/FontRenderer.h"
#include "gui/GUIButton.h"
#include "gui/GUIWindow.h"
#include "gui/GUIImage.h"
#include "gui/GUIPane.h"
#include "gui/GUIEditBox.h"
#include "gui/custom_elements/ToggleGroup.h"

#include "resources/MeshLoader.h"
#include "resources/ShaderLoader.h"
#include "resources/ImageLoader.h"
#include "scenegraph/SGGraphicsManager.h"
#include "scenegraph/SGScenegraph.h"
#include "scenegraph/ISGRenderQueue.h"
#include "scenegraph/SGScenegraphLoader.h"
#include "scenegraph/SGObjects.h"
#include "scenegraph/SGMaterial.h"
#include "scenegraph/SGSkyboxObject.h"

#include "MenuState.h"
#include "WaitForConnectionState.h"

menu_state::menu_state(state_manager* sm):game_state(sm)
{
    m_env=m_app_ctx->env;
    quads=new toggle_group();
    levels=new toggle_group();
    m_rot=0;
}

#define init_e(x) x->set_event_listener(this)
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

    m_app_ctx->gd->quad_choice=0;
    m_app_ctx->gd->current_level=0;

    gui_static_text* txt=(gui_static_text*)m_env->get_element_by_name("quad_info");
    txt->set_text(L"Orlaivio informacija: "+m_app_ctx->gd->quadcopter_descriptions[0].info);

    txt=(gui_static_text*)m_env->get_element_by_name("quad_difficulty");
    txt->set_text(L"Sudėtingumas: "+m_app_ctx->gd->quadcopter_descriptions[0].difficulty);

    txt=(gui_static_text*)m_env->get_element_by_name("quad_speed");
    txt->set_text(L"Greitis: "+m_app_ctx->gd->quadcopter_descriptions[0].speed);

    txt=(gui_static_text*)m_env->get_element_by_name("quad_mass");
    txt->set_text(L"Masė: "+m_app_ctx->gd->quadcopter_descriptions[0].mass);

    txt=(gui_static_text*)m_env->get_element_by_name("level_info");
    txt->set_text(L"Lygio informacija: "+m_app_ctx->gd->training_descriptions[0].info);

    txt=(gui_static_text*)m_env->get_element_by_name("level_quad");
    txt->set_text(L"Rekomenduojamas orlaivis: "+m_app_ctx->gd->training_descriptions[0].recommended_quad);

    create_scene();
}

void menu_state::create_scene()
{
    sg::sg_camera_object_ptr cam=sg::sg_camera_object_ptr(new sg::sg_camera_object(m_app_ctx,glm::vec3(0,0,-4),glm::vec3(0,0,0),glm::vec3(0,1,0),1.777777f,45.0f,1.0f,10000.f));
    m_app_ctx->sg->set_active_camera(cam);
    cam->orbit(glm::vec3(0,0,0),4,3.14/180,3.14/180);

    ///load model
    obj=m_app_ctx->sg->load_mesh_object("res/quadcopters/default_quad.iqm",false);
    //obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_app_ctx->sg,m));
    obj->set_position(glm::vec3(0,0,0));

    sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
    sm_mat->mat_texture= m_app_ctx->gm->load_texture("res/quadcopters/blue.png");

    printf("PING.\n");
    m_app_ctx->sg->add_object(obj);

    sg::sg_light_object_ptr light = m_app_ctx->sg->add_light_object();
    light->set_position(glm::vec3(0,100,0));

    skybox = m_app_ctx->sg->load_skybox("res/models/sky1/sky.iqm",true);
    m_app_ctx->sg->add_object(skybox);
}



void menu_state::create_quad_selections()
{
    pane=new gui_pane(m_env,rect2d<int>(10,50,580,220),true);
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

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/quadcopters/default_selection.png"));
    btn->set_image(img);

    st=new gui_static_text(m_env,rect2d<int>(10,110,50,20),L"Paprastasis",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(100,30,80,80),L"",true,false);
    btn->set_name("QS2");
    btn->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(btn);
    quads->add(btn);

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/quadcopters/unshielded_selection.png"));
    btn->set_image(img);

    st=new gui_static_text(m_env,rect2d<int>(100,110,50,20),L"Be apsaugos",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(190,30,80,80),L"",true,false);
    btn->set_name("QS3");
    btn->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(btn);
    quads->add(btn);

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/quadcopters/micro_selection.png"));
    btn->set_image(img);

    st=new gui_static_text(m_env,rect2d<int>(190,110,50,20),L"Mini",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(280,30,80,80),L"",true,false);
    btn->set_name("QS4");
    btn->set_parent(m_env->get_element_by_name("quad_panel"));
    init_e(btn);
    quads->add(btn);

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/quadcopters/fast_selection.png"));
    btn->set_image(img);

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
    st->set_name("quad_info");
    st->set_parent(m_env->get_element_by_name("quad_info_panel"));
    init_e(st);

    st=new gui_static_text(m_env,rect2d<int>(10,30,100,20),L"Sudėtingumas:",glm::vec4(1),false,true);
    st->set_name("quad_difficulty");
    st->set_parent(m_env->get_element_by_name("quad_info_panel"));
    init_e(st);

    st=new gui_static_text(m_env,rect2d<int>(10,50,100,20),L"Greitis:",glm::vec4(1),false,true);
    st->set_name("quad_speed");
    st->set_parent(m_env->get_element_by_name("quad_info_panel"));
    init_e(st);

    st=new gui_static_text(m_env,rect2d<int>(10,70,100,20),L"Mąsė:",glm::vec4(1),false,true);
    st->set_name("quad_mass");
    st->set_parent(m_env->get_element_by_name("quad_info_panel"));
    init_e(st);
}

void menu_state::create_level_selections()
{
    pane=new gui_pane(m_env,rect2d<int>(10,270,580,220),true);
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

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/maps/trainings/training1_selection.png"));
    btn->set_image(img);

    st=new gui_static_text(m_env,rect2d<int>(10,110,50,20),L"Treniruotė 1",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(100,30,80,80),L"",true,false);
    btn->set_name("LS2");
    btn->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(btn);
    levels->add(btn);

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/maps/trainings/training2_selection.png"));
    btn->set_image(img);

    st=new gui_static_text(m_env,rect2d<int>(100,110,50,20),L"Treniruotė 2",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(190,30,80,80),L"",true,false);
    btn->set_name("LS3");
    btn->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(btn);
    levels->add(btn);

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/maps/trainings/training3_selection.png"));
    btn->set_image(img);

    st=new gui_static_text(m_env,rect2d<int>(190,110,50,20),L"Treniruotė 3",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(280,30,80,80),L"",true,false);
    btn->set_name("LS4");
    btn->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(btn);
    btn->set_enabled(false);
    levels->add(btn);

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/maps/trainings/training4_selection.png"));
    btn->set_image(img);

    st=new gui_static_text(m_env,rect2d<int>(280,110,50,20),L"Treniruotė 4",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    btn=new gui_button(m_env,rect2d<int>(370,30,80,80),L"",true,false);
    btn->set_name("LS5");
    btn->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(btn);
    btn->set_enabled(false);
    levels->add(btn);

    img=new gui_image(m_env,rect2d<int>(20,20,40,40),m_app_ctx->gm->load_texture("res/maps/trainings/training5_selection.png"));
    btn->set_image(img);

    st=new gui_static_text(m_env,rect2d<int>(370,110,50,20),L"Treniruotė 5",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(st);

    pane=new gui_pane(m_env,rect2d<int>(5,125,450,90),true);
    pane->set_name("level_info_panel");
    pane->set_parent(m_env->get_element_by_name("level_panel"));
    init_e(pane);

    st=new gui_static_text(m_env,rect2d<int>(10,10,200,20),L"Lygio informacija:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_info_panel"));
    st->set_name("level_info");
    init_e(st);

    st=new gui_static_text(m_env,rect2d<int>(10,30,200,20),L"Orlaivis:",glm::vec4(1),false,true);
    st->set_parent(m_env->get_element_by_name("level_info_panel"));
    st->set_name("level_quad");
    init_e(st);
}

void menu_state::create_start_window()
{
    win=new gui_window(m_env,rect2d<int>(400,100,600,540),L"Pasirinkite orlaivį ir lygį",true,false,false,false);
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

    btn=new gui_button(m_env,rect2d<int>(260,495,80,30),L"Pradėti");
    btn->set_name("start_level");
    btn->set_parent(m_env->get_element_by_name("win_normal"));
    init_e(btn);

    win->set_visible(false);
}

void menu_state::on_unload()
{
    //m_app_ctx->se->stopAllSounds();
    m_app_ctx->sm->stop_sound("menu_music");
}

void menu_state::start()
{
    m_app_ctx->sm->add_sound("res/sounds/gui_select.ogg","gui_hover",0.75,1);
    m_app_ctx->sm->add_sound("res/sounds/gui_click.ogg","gui_click",0.75,1);
    m_app_ctx->sm->add_sound("res/sounds/main.ogg","menu_music",0.75,1);
    m_app_ctx->sm->play_sound_2d("menu_music",false);
}

void menu_state::update(float delta)
{
    m_rot+=delta*10;
    m_env->update(delta);
    glm::quat q=glm::quat();
    q=glm::rotate(q,-20,glm::vec3(1,0,0));
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
            m_app_ctx->sm->play_sound_2d("gui_hover",false);
        break;
    case button_pressed:
        if(e.get_caller()->get_element_type()==GUIET_button)
            m_app_ctx->sm->play_sound_2d("gui_click",false);
        break;
    case button_released:
        if(e.get_caller()->get_name().find("QS")!=std::string::npos)
        {
            int sel=atoi(e.get_caller()->get_name().substr(2).c_str())-1;
            quads->set_toggle(sel);
            m_app_ctx->gd->quad_choice=sel;

            gui_static_text* txt=(gui_static_text*)m_env->get_element_by_name("quad_info");
            txt->set_text(L"Orlaivio informacija: "+m_app_ctx->gd->quadcopter_descriptions[sel].info);

            txt=(gui_static_text*)m_env->get_element_by_name("quad_difficulty");
            txt->set_text(L"Sudėtingumas: "+m_app_ctx->gd->quadcopter_descriptions[sel].difficulty);

            txt=(gui_static_text*)m_env->get_element_by_name("quad_speed");
            txt->set_text(L"Greitis: "+m_app_ctx->gd->quadcopter_descriptions[sel].speed);

            txt=(gui_static_text*)m_env->get_element_by_name("quad_mass");
            txt->set_text(L"Masė: "+m_app_ctx->gd->quadcopter_descriptions[sel].mass);

            return true;
        }
        if(e.get_caller()->get_name().find("LS")!=std::string::npos)
        {
            int sel=atoi(e.get_caller()->get_name().substr(2).c_str())-1;
            levels->set_toggle(sel);
            m_app_ctx->gd->set_level(sel);

            gui_static_text* txt=(gui_static_text*)m_env->get_element_by_name("level_info");
            txt->set_text(L"Lygio informacija: "+m_app_ctx->gd->training_descriptions[sel].info);

            txt=(gui_static_text*)m_env->get_element_by_name("level_quad");
            txt->set_text(L"Rekomenduojamas orlaivis: "+m_app_ctx->gd->training_descriptions[sel].recommended_quad);
            return true;
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

