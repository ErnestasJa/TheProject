#include "precomp.h"

#include "Application/Window.h"
#include "opengl/opengl_util.h"
#include "utility/timer.h"
#include "utility/logger.h"

#include "Application.h"

void Application::output_versions()
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
    this->_window = nullptr;
    this->_GLUtil = nullptr;
    this->_mainTimer = nullptr;
    this->_argc = argc;
    this->_argv = argv;
}

Application::~Application()
{

}

bool Application::init(const std::string  &title, uint32_t width, uint32_t height)
{
    this->_mainTimer = timer_ptr(new timer());
    if (!PHYSFS_init(argv[0]))
    {
        std::cout<<"PHYSFS_init() failed: " <<PHYSFS_getLastError()<<std::endl;
        return false;
    }

    std::string real_dir = PHYSFS_getBaseDir();

    std::string dir;

    uint32_t pos = real_dir.find_last_of(PHYSFS_getDirSeparator());
    real_dir = real_dir.substr(0,pos);

    pos = real_dir.find_last_of(PHYSFS_getDirSeparator());
    real_dir = real_dir.substr(0,pos);

    pos = real_dir.find_last_of(PHYSFS_getDirSeparator());
    dir = real_dir.substr(0,pos);

    dir+=PHYSFS_getDirSeparator();

    m_log=new logger(this,0);
    m_log->log(LOG_LOG,"Initializing \"%s\"",title.c_str());

    m_log->log(LOG_LOG,"Base Directory: \"%s\"",PHYSFS_getBaseDir());
    m_log->log(LOG_LOG,"Directory: \"%s\"",dir.c_str());

    #ifdef RELEASE_FS
    PHYSFS_mount(PHYSFS_getBaseDir(),NULL,0);
    #else
    PHYSFS_mount(dir.c_str(), NULL, 0);
    #endif // RELEASE_FS

    std::string combo=PHYSFS_getBaseDir();
    combo+="res/";
    PHYSFS_mount(combo.c_str(),NULL,0);
    output_versions();

    this->wnd = new Window();

    if(!this->wnd->init(title, width, height))
    {
        delete wnd;
        return false;
    }

    this->wnd->sig_window_closed().connect(sigc::mem_fun(this,&Application::on_window_close));

    this->gl_util = new opengl_util(m_log);

    if(!this->gl_util->load_extensions())
    {
        delete wnd;
        delete gl_util;
        return false;
    }

    return true;
}

void Application::exit()
{
    m_log->log(LOG_LOG,"Exitting.");

    delete gl_util;
    Window::destroy_window(wnd);
    delete m_log;

    if (!PHYSFS_deinit())
        std::cout<< "PHYSFS_deinit() failed!\n reason: " << PHYSFS_getLastError() << "." << std::endl;

    main_timer = nullptr;
}

logger *Application::get_logger()
{
    return m_log;
}

timer_ptr Application::get_timer()
{
    return main_timer;
}
