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

play_state::play_state(state_manager* sm):game_state(sm)
{
    m_env=m_app_ctx->env;
}

#define init_e(x) x->set_event_listener(this)
void play_state::on_load()
{
    gui_element* el=new gui_button(m_env,rect2d<int>(0,0,100,20));
    init_e(el);
}

void play_state::on_unload()
{
//    for(gui_element* el:elems)
//    {
//        m_env->remove_child(el);
//    }
    printf("UNLOADING STATE\n");
    m_env->destroy_children();
    //m_app_ctx->se->stopAllSounds();
}

void play_state::start()
{
    sg::sg_camera_object_ptr cam=sg::sg_camera_object_ptr(new sg::sg_camera_object(m_app_ctx->sg,glm::vec3(0,5,-10),glm::vec3(0,0,0),glm::vec3(0,1,0),1.777777f,45.0f,1.0f,10000.f));
    m_app_ctx->sg->set_active_camera(cam);

    sg::sg_light_object_ptr light = m_app_ctx->sg->add_light_object();
    light->set_position(glm::vec3(0,100,0));

    sg::sg_skybox_object_ptr skybox = m_app_ctx->sg->load_skybox("res/models/sky1/sky.iqm",true);
    m_app_ctx->sg->add_object(skybox);

    m_quadcopter=new sg::quadcopter(m_app_ctx,QUAD_MODEL_DEFAULT,btVector3(0,0,0),10,btVector3(0,0,0),true);
}

void play_state::update(float delta)
{
    m_env->update(delta);
    m_quadcopter->update(delta);
    m_quadcopter->variate_axis(m_app_ctx->nm->get_rot_diff());
    m_quadcopter->variate_gyro(m_app_ctx->nm->get_accelerometer_data());
    m_quadcopter->variate_height(m_app_ctx->nm->get_height_diff());
    sg::sg_camera_object_ptr cam=m_app_ctx->sg->get_active_camera();
    cam->orbit(physics_manager::bt_to_glm(m_quadcopter->m_transform),10,80,180+m_quadcopter->m_rotation.y()*180.f/glm::pi<float>());
}

bool play_state::on_event(const gui_event &e)
{
    switch(e.get_type())
    {
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
}

