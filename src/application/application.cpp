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
    this->_window = nullptr;
    this->_GLUtil = nullptr;
    this->_mainTimer = nullptr;
    this->_argc = argc;
    this->_argv = argv;
}

Application::~Application()
{

}

bool Application::Init(const std::string  &title, uint32_t width, uint32_t height)
{
    this->_mainTimer = timer_ptr(new Timer());
    if (!PHYSFS_init(_argv[0]))
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

    _logger=new logger(this,0);
    _logger->log(LOG_LOG,"Initializing \"%s\"",title.c_str());

    _logger->log(LOG_LOG,"Base Directory: \"%s\"",PHYSFS_getBaseDir());
    _logger->log(LOG_LOG,"Directory: \"%s\"",dir.c_str());

    #ifdef RELEASE_FS
    PHYSFS_mount(PHYSFS_getBaseDir(),NULL,0);
    #else
    PHYSFS_mount(dir.c_str(), NULL, 0);
    #endif // RELEASE_FS

    std::string combo=PHYSFS_getBaseDir();
    combo+="res/";
    PHYSFS_mount(combo.c_str(),NULL,0);
    OutputVersions();

    this->_window = new Window();

    if(!this->_window->Init(title, width, height))
    {
        delete _window;
        return false;
    }

    this->_window->SigWindowClosed().connect(sigc::mem_fun(this,&Application::OnWindowClose));

    this->_GLUtil = new OpenGLUtil(_logger);

    if(!this->_GLUtil->load_extensions())
    {
        delete _window;
        delete _GLUtil;
        return false;
    }

    return true;
}

void Application::Exit()
{
    _logger->log(LOG_LOG,"Exitting.");

    delete _GLUtil;
    Window::DestroyWindow(_window);
    delete _logger;

    if (!PHYSFS_deinit())
        std::cout<< "PHYSFS_deinit() failed!\n reason: " << PHYSFS_getLastError() << "." << std::endl;

    _mainTimer = nullptr;
}

logger *Application::GetLogger()
{
    return _logger;
}

timer_ptr Application::GetTimer()
{
    return _mainTimer;
}
