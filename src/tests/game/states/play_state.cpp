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

#include "../physics/Physics.h"

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

#include "../quadcopter.h"

#include "play_state.h"
#include "menu_state.h"

#include "../target_queue.h"

play_state::play_state(state_manager* sm):game_state(sm)
{
    m_env=m_app_ctx->env;
}

#define init_e(x) x->set_event_listener(this)
void play_state::on_load()
{
    can_proceed=false;
    load_according_to_game_data();
}

void play_state::on_unload()
{
    m_app_ctx->sm->stop_sound("quad_idle");
}

void play_state::load_according_to_game_data()
{
    game_data* gd=m_app_ctx->gd;

    sg::sg_scenegraph_loader l;

    switch(gd->current_level)
    {
    case 0:
        l.load_scene(m_app_ctx,"res/maps/trainings/training1.ibs",true);
        break;
    case 1:
        l.load_scene(m_app_ctx,"res/maps/trainings/training2.ibs",true);
        break;
    case 2:
        l.load_scene(m_app_ctx,"res/maps/trainings/training3.ibs",true);
        break;
    case 3:
        l.load_scene(m_app_ctx,"res/maps/trainings/training4.ibs",true);
        break;
    case 4:
        l.load_scene(m_app_ctx,"res/maps/trainings/training5.ibs",true);
        break;
    default:
        exit(-1);
        break;
    }

}

void play_state::start()
{
    sg::sg_camera_object_ptr cam=sg::sg_camera_object_ptr(new sg::sg_camera_object(m_app_ctx->sg,glm::vec3(0,5,-10),glm::vec3(0,0,0),glm::vec3(0,1,0),1.777777f,45.0f,1.0f,10000.f));

    m_app_ctx->sg->set_active_camera(cam);

    sg::sg_light_object_ptr light = m_app_ctx->sg->add_light_object();
    light->set_position(glm::vec3(0,100,0));

    sg::sg_skybox_object_ptr skybox = m_app_ctx->sg->load_skybox("res/models/sky1/sky.iqm",true);
    m_app_ctx->sg->add_object(skybox);

    sg::sg_mesh_object_ptr arrow=m_app_ctx->sg->load_mesh_object("res/quadcopters/arrow.iqm",true);
    m_app_ctx->sg->add_object(arrow);

    std::vector<trigger> points=m_app_ctx->sg->get_triggers();
    std::vector<glm::vec3> topush;

    glm::vec3 s,f;
    std::vector<trigger>::iterator it=points.begin(),end=points.end();
    for(; it!=end; )
    {
        if((*it).name=="Spawnpoint")
        {
            s=(*it).obj->get_position();
            it=points.erase(it);
            end=points.end();
        }
        else if((*it).name=="Target")
        {
            f=(*it).obj->get_position();
            it=points.erase(it);
            end=points.end();
        }
        else
            it++;
    }
    std::sort(points.begin(),points.end());
    for(trigger trig:points)
    {
        topush.push_back(trig.obj->get_position());
    }
    m_quadcopter=new sg::quadcopter(m_app_ctx,m_app_ctx->gd->quad_choice,m_app_ctx->pm->glm_to_bt(s),1,btVector3(0,0,0),true);
    m_quadcopter->set_rotation(glm::vec3(0,180-45,0));
    m_target_queue=new target_queue(m_app_ctx,s,f,topush,arrow,m_quadcopter);
    m_target_queue->reset();
}

void play_state::update(float delta)
{
    if(!m_target_queue->is_completed()&&run)
    {
        m_app_ctx->gd->time++;
        m_target_queue->update();
        if(!m_app_ctx->nm->is_receiving())
            m_state_manager->set_state(new menu_state(m_state_manager));
        m_env->update(delta);
        m_quadcopter->variate_axis(m_app_ctx->nm->get_rot_diff());
        m_quadcopter->variate_gyro(m_app_ctx->nm->get_accelerometer_data());
        m_quadcopter->variate_height(m_app_ctx->nm->get_height_diff());
        m_quadcopter->update(delta);
        sg::sg_camera_object_ptr cam=m_app_ctx->sg->get_active_camera();
        cam->orbit(physics_manager::bt_to_glm(m_quadcopter->m_transform),5,80,180+m_quadcopter->m_rotation.y()*180.f/glm::pi<float>());
        m_app_ctx->pm->update(delta);
    }
    else if(m_target_queue->is_completed()&&m_app_ctx->gd->advance_level())
    {
        if(m_app_ctx->gd->set_level(m_app_ctx->gd->current_level+1))
        {
            m_state_manager->set_state(new play_state(m_state_manager));
        }
        else
        {
            m_state_manager->set_state(new menu_state(m_state_manager));
        }
    }
}

bool play_state::on_event(const gui_event &e)
{
    switch(e.get_type())
    {
    case gui_event_type::window_closed:
        can_proceed=true;
        break;
    default:
        break;
    }
    return false;
}

void play_state::render()
{
    glClearColor(0.2,0.2,0.4,1);
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    m_app_ctx->sg->render_all();
    m_env->render();
    m_env->get_font_renderer()->use_font("s42");
    wchar_t targets[16];
    swprintf_s(targets,L"%d/%d",m_target_queue->current(),m_target_queue->max());
    m_env->get_font_renderer()->render_string(std::wstring(targets),glm::vec2(600,20),glm::vec4(1),true);
    m_env->get_font_renderer()->use_font("default");
}

