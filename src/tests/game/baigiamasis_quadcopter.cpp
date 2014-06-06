#include "precomp.h"
#include "baigiamasis_quadcopter.h"

#include "application/window.h"

#include "resources/resource_cache.h"
#include "opengl/texture.h"
#include "opengl/render_buffer_object.h"
#include "opengl/frame_buffer_object.h"

#include "utility/timer.h"
#include "utility/logger.h"
#include "opengl/opengl_util.h"
#include "opengl/mesh.h"
#include "opengl/shader.h"

#include "resources/mesh_loader.h"
#include "resources/shader_loader.h"
#include "resources/image_loader.h"
#include "scenegraph/sg_graphics_manager.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/isg_render_queue.h"
#include "scenegraph/sg_scenegraph_loader.h"
#include "scenegraph/sg_objects.h"
#include "scenegraph/sg_material.h"
#include "physics/Physics.h"

#include "gui/gui_environment.h"

#include "states/state_manager.h"
#include "states/menu_state.h"
#include "states/play_state.h"
#include "app_context.h"

baigiamasis_quadcopter::baigiamasis_quadcopter(uint32_t argc, const char ** argv): application(argc,argv)
{
    //ctor
}

baigiamasis_quadcopter::~baigiamasis_quadcopter()
{
    //dtor

}

bool baigiamasis_quadcopter::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);
    wnd->sig_key_event().connect(sigc::mem_fun(this,&baigiamasis_quadcopter::on_key_event));


    m_scenegraph = new sg::scenegraph(this->get_logger(),this->get_timer());
    m_graphics_manager = m_scenegraph->get_graphics_manager();

    m_physics_manager = new physics_manager(btVector3(0,-9.83f,0));

    env=new gui_environment(this->wnd,this->get_logger());

    main_timer->tick();

    m_current_time = m_last_time = main_timer->get_real_time();

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	ctx=new app_context();
	ctx->win=this->wnd;
	ctx->gm=this->m_graphics_manager;
	ctx->sg=this->m_scenegraph;
	ctx->pm=this->m_physics_manager;
	ctx->env=this->env;
	ctx->gd=new game_data();
	ctx->nm=new network_manager_win32();
	ctx->sm=new sound_manager();

	ctx->env->get_font_renderer()->create_font("s22","freesans.ttf",22);
    ctx->env->get_font_renderer()->create_font("s42","freesans.ttf",42);

	m_state_manager=new state_manager(ctx);
	m_state_manager->set_state(new menu_state(m_state_manager));

    return !this->gl_util->check_and_output_errors();
}

bool baigiamasis_quadcopter::update()
{
    if(wnd->update() && m_state_manager->run)
    {
        // Measure speed
        main_timer->tick();
        m_last_time = m_current_time;
        m_current_time = main_timer->get_real_time();
        float delta_time = ((float)(m_current_time - m_last_time)) / 1000.0f;

        m_state_manager->update(delta_time);
        m_state_manager->render();

        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;

}

void baigiamasis_quadcopter::exit()
{
    m_graphics_manager = nullptr;

    delete m_state_manager;

    //delete env;
    delete ctx;
    application::exit();
}

bool baigiamasis_quadcopter::on_event(const gui_event & e)
{
    return false;
}

void baigiamasis_quadcopter::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{
}

void baigiamasis_quadcopter::on_mouse_move(double x, double y)
{
}
