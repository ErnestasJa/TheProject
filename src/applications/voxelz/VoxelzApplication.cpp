#include "Precomp.h"
#include "VoxelzApplication.h"

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
#include "../../physics/Physics.h"

#include "gui/GUIEnvironment.h"

#include "../AppContext.h"

voxelz_Application::voxelz_Application(uint32_t argc, const char ** argv): Application(argc,argv)
{
    //ctor
}

voxelz_Application::~voxelz_Application()
{
    //dtor

}

bool voxelz_Application::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);
    _window->SigKeyEvent().connect(sigc::mem_fun(this,&voxelz_Application::on_key_event));


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

    return !this->_GLUtil->check_and_output_errors();
}

bool voxelz_Application::Update()
{
    if(_window->Update())
    {
        // Measure speed
        _mainTimer->tick();
        m_last_time = m_current_time;
        m_current_time = _mainTimer->get_real_time();
        float delta_time = ((float)(m_current_time - m_last_time)) / 1000.0f;

//        m_state_manager->update(delta_time);
//        m_state_manager->render();

        _window->swap_buffers();

        ///let's just rage quit on gl error
        return !this->_GLUtil->check_and_output_errors();
    }
    return false;

}

void voxelz_Application::Exit()
{
    m_graphics_manager = nullptr;

    delete m_state_manager;

    //delete env;
    delete ctx;
    Application::Exit();
}

bool voxelz_Application::on_event(const gui_event & e)
{
    return false;
}

void voxelz_Application::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{
}

void voxelz_Application::on_mouse_move(double x, double y)
{
}
