#include "Precomp.h"

#include "application/Window.h"
#include "utility/Timer.h"
#include "utility/Logger.h"
#include "utility/StringUtil.h"

#include "Application.h"

void Application::OutputPhysFSVersions()
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
    this->_argc = argc;
    this->_argv = argv;
    _appContext = new AppContext();
}

Application::~Application()
{

}

bool Application::Init(const std::string  &title, uint32_t width, uint32_t height)
{
    _appContext->_timer = timer_ptr(new Timer());

    if(!PHYSFS_init(_argv[0]))
    {
        std::cout<<"PHYSFS_init() failed: " <<PHYSFS_getLastError()<<std::endl;
        return false;
    }

    _appContext->_logger=new Logger(this,0);

    OutputPhysFSVersions();

    _workingDirectoryPath = PHYSFS_getBaseDir();

    #ifndef RELEASE_FS
        loopi(4)
        _workingDirectoryPath = util::GetParentDirectory(_workingDirectoryPath, PHYSFS_getDirSeparator());
        _workingDirectoryPath += PHYSFS_getDirSeparator();
    #endif

    PHYSFS_mount(_workingDirectoryPath.c_str(), NULL, 0);
    _appContext->_logger->log(LOG_LOG,"Directory: \"%s\"", _workingDirectoryPath.c_str());

    _resourcePath = _workingDirectoryPath + "res" + PHYSFS_getDirSeparator();
    PHYSFS_mount(_resourcePath.c_str(),NULL,0);

    _appContext->_logger->log(LOG_LOG,"Setting write dir: \"%s\"", _resourcePath.c_str());
    int32_t changeWriteDirStatus = PHYSFS_setWriteDir(_resourcePath.c_str());

    if(changeWriteDirStatus == 0)
        _appContext->_logger->log(LOG_ERROR,"Write dir change failed.");
    else
        _appContext->_logger->log(LOG_LOG,"Write dir is: \"%s\"", PHYSFS_getWriteDir());


    _appContext->_window = new Window();

    if(!_appContext->_window->Init(title, width, height))
    {
        delete _appContext->_window;
        _appContext->_window = nullptr;
        return false;
    }

    _appContext->_window->SigWindowClosed().connect(sigc::mem_fun(this,&Application::OnWindowClose));

    this->_appContext->_glUtil = new OpenGLUtil(_appContext->_logger);

    if(!this->_appContext->_glUtil->load_extensions())
    {
        delete _appContext->_window;
        return false;
    }

    _appContext->_logger->log(LOG_CRITICAL, "Shading language: %s", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

    if(!_appContext->IsInitialized())
        throw "Failed to initialize app context.";

    return true;
}

void Application::Exit()
{
    _appContext->_logger->log(LOG_LOG,"Exitting.");

    Window::DestroyWindow(_appContext->_window);
    delete _appContext->_logger;

    if (!PHYSFS_deinit())
        std::cout<< "PHYSFS_deinit() failed!\n reason: " << PHYSFS_getLastError() << "." << std::endl;

    _appContext->_timer = nullptr;
}

AppContext * Application::Ctx()
{
    return _appContext;
}

std::string Application::GetAbsoluteResourcePath()
{
    return _resourcePath;
}

std::string Application::GetAbsoluteWorkingDirectoryPath()
{
    return _workingDirectoryPath;
}
