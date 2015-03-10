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

Application::Application(int32_t argc, const char ** argv): VarGroup("settings")
{
    this->_argc = argc;
    this->_argv = argv;
    _appContext = new AppContext();
}

#define DSP() PHYSFS_getDirSeparator()
void Application::InitSettings()
{
    VarGroup & fs = this->AddGroup("filesystem");
    fs.AddVar(Var("resource_path", "res"));
    fs.AddVar(Var("engine_resource_path", "engine"));
    fs.AddVar(Var("config_path", "conf"));
    fs.AddVar(Var("log_path", "log"));
    Application::LoadSettings(*this, (std::string(fs.GetVar("config_path").ValueS()) + DSP() + "config.json").c_str());

    InitVariables();
    VarGroup & app = this->GetGroup(this->GetApplicationId().c_str());

    if(app.Name()!=0)
        Application::LoadSettings(app, (std::string(fs.GetVar("config_path").ValueS()) + DSP() + this->GetApplicationId() + ".json").c_str());
}

void Application::ApplySettings()
{
    InitFileSystem();
}

void Application::SetWriteDirectory(const std::string & dir)
{
    int32_t changeWriteDirStatus = PHYSFS_setWriteDir(dir.c_str());

    if(changeWriteDirStatus == 0){
        printf("Write dir change to '%s' failed.", dir.c_str());
        exit(-1);
    }
}

void Application::InitFileSystem()
{
    SetWriteDirectory(_workingDirectoryPath);

    //! Add error handling/exceptions
    VarGroup & fs = this->GetGroup("filesystem");

    ///first set working directory as write dir
    _resourcePath = std::string(fs.GetVar("resource_path").ValueS()) + DSP();
    std::string engineResources = _resourcePath + fs.GetVar("engine_resource_path").ValueS() + DSP();
    std::string appWriteDir = _resourcePath + this->GetApplicationId() + DSP();
    std::string appLogDir = appWriteDir + fs.GetVar("log_path").ValueS() + DSP();
    std::string appConfigPath = appWriteDir + fs.GetVar("config_path").ValueS() + DSP();

    PHYSFS_mkdir(engineResources.c_str());
    PHYSFS_mkdir(appWriteDir.c_str());
    PHYSFS_mkdir(appLogDir.c_str());
    PHYSFS_mkdir(appConfigPath.c_str());

    PHYSFS_mount(_resourcePath.c_str(),NULL,0);
    SetWriteDirectory(_workingDirectoryPath+appWriteDir);
}

#undef DSP

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

    OutputPhysFSVersions();

    _workingDirectoryPath = PHYSFS_getBaseDir();
    PHYSFS_mount(_workingDirectoryPath.c_str(), NULL, 0);

    InitSettings();
    ApplySettings();

    _appContext->_logger = new Logger(this,0);
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


VarGroup & Application::GetEngineVars()
{
    return *this;
}

VarGroup & Application::GetAppVars()
{
    return this->GetGroup(this->GetApplicationId().c_str());
}

void Application::LoadSettings(VarGroup & group, const std::string & settingsFile, Logger * log)
{
    char * buf;
    uint32_t len;
    len=helpers::read(settingsFile.c_str(),buf);

    if(len>0)
    {
        VarJsonReader reader(log);
        reader.Load(buf,len,group);
    }
}
