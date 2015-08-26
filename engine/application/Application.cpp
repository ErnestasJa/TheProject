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

#define DSP() "/"
void Application::InitSettings()
{
    VarGroup & fs = this->AddGroup("filesystem");
    fs.AddVar(Var("resource_path", "res"));
    fs.AddVar(Var("engine_resource_path", "engine"));
    fs.AddVar(Var("config_path", "conf"));
    fs.AddVar(Var("log_path", "log"));
    Application::LoadSettings(*this, "config.json");

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

void RemoveFromEnd(std::string & stringToSearch, const std::string & toRemove)
{
    if(&stringToSearch[stringToSearch.length()-toRemove.length()]==toRemove){
        stringToSearch.erase((stringToSearch.length()-toRemove.length()),toRemove.length());
    }
}

#include <boost/algorithm/string/replace.hpp>
std::string Application::GetAbsoluteDir(const std::string & absolutePart, const std::string & relativePart)
{
    ///Maybe add regex path validations, throw exceptions?
    std::string dir = relativePart,
        absoluteDir = absolutePart,
        backStr = std::string("..") + DSP();

    RemoveFromEnd(dir,DSP());
    RemoveFromEnd(absoluteDir,DSP());

    uint32_t len = dir.size();

    boost::replace_all(dir, backStr, "");

    uint32_t removed = (len-dir.size())/backStr.length();
    std::string::size_type index = std::string::npos;

    for(uint32_t i = 0; i < removed; i++)
    {
        uint32_t found = absoluteDir.find_last_of(DSP(),index-1);
        if(found<index)
            index = found;
    }

    if(index<std::string::npos)
        absoluteDir.erase(absoluteDir.begin()+index,absoluteDir.end());

    RemoveFromEnd(absoluteDir,DSP());
    return absoluteDir;
}

void Application::InitFileSystem()
{
    // Working dir
    // /home/senpai/Coding/TheProject/src/applications/voxel_octree/bin
    // Res dir
    // /home/senpai/Coding/TheProject/src/applications/voxel_octree/resources
    //

    //! Add error handling/exceptions
    VarGroup & fs = this->GetGroup("filesystem");

    _resourcePath = std::string(fs.GetVar("resource_path").ValueS());
    _workingDirectoryPath = GetAbsoluteDir(_workingDirectoryPath, _resourcePath);
    boost::replace_all(_resourcePath, std::string("..") + DSP(), "");

    ///first set working directory as write dir
    std::string engineResources = _resourcePath + DSP() + fs.GetVar("engine_resource_path").ValueS();
    std::string appWriteDir = _resourcePath + DSP() + this->GetApplicationId();
    std::string appLogDir = appWriteDir + DSP() + fs.GetVar("log_path").ValueS();
    std::string appConfigPath = appWriteDir + DSP() + fs.GetVar("config_path").ValueS();

    SetWriteDirectory(_workingDirectoryPath);

    PHYSFS_mkdir(_resourcePath.c_str());
    PHYSFS_mkdir(engineResources.c_str());
    PHYSFS_mkdir(appWriteDir.c_str());
    PHYSFS_mkdir(appLogDir.c_str());
    PHYSFS_mkdir(appConfigPath.c_str());

    PHYSFS_mount((_workingDirectoryPath +DSP()+ _resourcePath).c_str(),NULL,0);
    PHYSFS_mount((_workingDirectoryPath +DSP()+ engineResources).c_str(),NULL,0);
    PHYSFS_mount((_workingDirectoryPath +DSP()+ appWriteDir).c_str(),NULL,0);

    SetWriteDirectory(_workingDirectoryPath +DSP()+ appWriteDir);
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

    OutputPhysFSVersions();

    _workingDirectoryPath = PHYSFS_getBaseDir();
    PHYSFS_mount(_workingDirectoryPath.c_str(), NULL, 0);

    boost::replace_all(_workingDirectoryPath, PHYSFS_getDirSeparator(), DSP());
    RemoveFromEnd(_workingDirectoryPath, DSP());

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
