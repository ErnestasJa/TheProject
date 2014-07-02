#include "Precomp.h"
#include "BaigiamasisQuadcopter.h"

#include "Application/Window.h"

#include "resources/ResourceCache.h"
#include "opengl/Texture.h"
#include "opengl/RenderBufferObject.h"
#include "opengl/FrameBufferObject.h"

#include "utility/Timer.h"
#include "utility/Logger.h"
#include "opengl/OpenGLUtil.h"
#include "opengl/Mesh.h"
#include "opengl/Shader.h"

#include "resources/MeshLoader.h"
#include "resources/ShaderLoader.h"
#include "resources/ImageLoader.h"
#include "scenegraph/SGGraphicsManager.h"
#include "scenegraph/Scenegraph.h"
#include "scenegraph/ISGRenderQueue.h"
#include "scenegraph/SGScenegraphLoader.h"
#include "scenegraph/SGObjects.h"
#include "scenegraph/SGMaterial.h"
#include "physics/Physics.h"

#include "gui/GUIEnvironment.h"

#include "states/StateManager.h"
#include "states/MenuState.h"
#include "states/PlayState.h"
#include "../AppContext.h"

baigiamasis_quadcopter::baigiamasis_quadcopter(uint32_t argc, const char ** argv): Application(argc,argv)
{
    //ctor
}

baigiamasis_quadcopter::~baigiamasis_quadcopter()
{
    //dtor

}

bool baigiamasis_quadcopter::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);
    _window->SigKeyEvent().connect(sigc::mem_fun(this,&baigiamasis_quadcopter::OnKeyEvent));


    m_scenegraph = new sg::scenegraph(this->GetLogger(),this->GetTimer());
    m_graphics_manager = m_scenegraph->get_graphics_manager();

    m_physics_manager = new physics_manager(btVector3(0,-9.83f,0));

    env=new gui_environment(this->_window,this->GetLogger());

    _mainTimer->tick();

    m_current_time = m_last_time = _mainTimer->get_real_time();

    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	ctx=new app_context();
	ctx->win=this->_window;
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

    return !this->_GLUtil->check_and_output_errors();
}

bool baigiamasis_quadcopter::Update()
{
    if(_window->getWindowShouldClose()==false && m_state_manager->run)
    {
        // Measure speed
        _mainTimer->tick();
        m_last_time = m_current_time;
        m_current_time = _mainTimer->get_real_time();
        float delta_time = ((float)(m_current_time - m_last_time)) / 1000.0f;

        m_state_manager->update(delta_time);
        m_state_manager->render();

        _window->swap_buffers();

        ///let's just rage quit on gl error
        return !this->_GLUtil->check_and_output_errors();
    }
    return false;

}

void baigiamasis_quadcopter::Exit()
{
    m_graphics_manager = nullptr;

    delete m_state_manager;

    //delete env;
    delete ctx;
    Application::Exit();
}

bool baigiamasis_quadcopter::on_event(const gui_event & e)
{
    return false;
}

void baigiamasis_quadcopter::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{
}

void baigiamasis_quadcopter::OnMouseMove(double x, double y)
{
}

void baigiamasis_quadcopter::OnWindowClose()
{
    this->_window->setWindowShouldClose(true);
}
