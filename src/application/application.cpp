#include "Precomp.h"

#include "Application/Window.h"
#include "opengl/OpenGLUtil.h"
#include "utility/Timer.h"
#include "utility/Logger.h"

#include "Application.h"

void Application::OutputVersions()
{
    PHYSFS_Version compiled;
    PHYSFS_Version linked;

    PHYSFS_VERSION(&compiled);
    PHYSFS_getLinkedVersion(&linked);

    printf("Compiled against PhysicsFS version %d.%d.%d,\n"
           " and linked against %d.%d.%d.\n\n",
            (int) compiled.major, (int) compiled.minor, (int) compiled.patch,
            (int) linked.major, (int) linked.minor, (int) linked.patch);
} /* output_versions */

Application::Application(int32_t argc, const char ** argv)
{
    this->_GLUtil = nullptr;
    this->_argc = argc;
    this->_argv = argv;
    m_app_context = new app_context();
}

Application::~Application()
{

}

bool Application::Init(const std::string  &title, uint32_t width, uint32_t height)
{

    m_app_context->app_timer = timer_ptr(new timer());
    {
        std::cout<<"PHYSFS_init() failed: " <<PHYSFS_getLastError()<<std::endl;
        return false;
    }

    std::string realDir = PHYSFS_getBaseDir();

    std::string dir;

    uint32_t pos = realDir.find_last_of(PHYSFS_getDirSeparator());
    realDir = realDir.substr(0,pos);

    pos = realDir.find_last_of(PHYSFS_getDirSeparator());
    realDir = realDir.substr(0,pos);

    pos = realDir.find_last_of(PHYSFS_getDirSeparator());
    dir = realDir.substr(0,pos);

    dir+=PHYSFS_getDirSeparator();

    m_app_context->log=new logger(this,0);
    m_app_context->log->log(LOG_LOG,"Initializing \"%s\"",title.c_str());

    m_app_context->log->log(LOG_LOG,"Base Directory: \"%s\"",PHYSFS_getBaseDir());
    m_app_context->log->log(LOG_LOG,"Directory: \"%s\"",dir.c_str());

    #ifdef RELEASE_FS
    PHYSFS_mount(PHYSFS_getBaseDir(),NULL,0);
    #else
    PHYSFS_mount(dir.c_str(), NULL, 0);
    #endif // RELEASE_FS

    std::string combo=PHYSFS_getBaseDir();
    combo+="res/";
    PHYSFS_mount(combo.c_str(),NULL,0);
    OutputVersions();

    m_app_context->app_window = new window();

    if(!m_app_context->app_window->init(title, width, height))
    {
        delete m_app_context->app_window;
        m_app_context->app_window = nullptr;
        return false;
    }

    m_app_context->app_window->sig_window_closed().connect(sigc::mem_fun(this,&application::on_window_close));

    this->gl_util = new opengl_util(m_app_context->log);

    if(!this->_GLUtil->load_extensions())
    {
        delete m_app_context->app_window;
        delete _GLUtil;
        return false;
    }

    m_app_context->scenegraph = new sg::sg_scenegraph(m_app_context->log, m_app_context->app_timer);
    m_app_context->graphics_manager = m_app_context->scenegraph->get_graphics_manager();

    m_app_context->log->log(LOG_CRITICAL,(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    return true;
}

void Application::Exit()
{
    m_app_context->log->log(LOG_LOG,"Exitting.");

    delete _GLUtil;
    window::destroy_window(m_app_context->app_window);
    delete m_app_context->log;

    if (!PHYSFS_deinit())
        std::cout<< "PHYSFS_deinit() failed!\n reason: " << PHYSFS_getLastError() << "." << std::endl;

    m_app_context->app_timer = nullptr;
}

logger *Application::GetLogger()
{
    return m_app_context->log;
}

timer_ptr Application::GetTimer()
{
    return m_app_context->app_timer;
}
