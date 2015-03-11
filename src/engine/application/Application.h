#pragma once

class Window;
class OpenGLUtil;
class Logger;

#include "AppContext.h"

#include "core/VarJsonReader.h"
#include "core/Var.h"
#include "core/VarGroup.h"

class Application: public VarGroup
{
protected:
    ///remove these after variables fully work
    std::string _resourcePath;
    std::string _workingDirectoryPath;
    std::string _logPath;

    AppContext * _appContext;
    int32_t _argc;
    const char ** _argv;
    virtual void OutputPhysFSVersions();

    void InitSettings();
    void InitFileSystem();
    void SetWriteDirectory(const std::string & dir);
    std::string GetAbsoluteDir(const std::string & absoluteDir,const std::string & relativeDir);
    void ApplySettings();
public:

    Application(int32_t argc, const char ** argv);
    virtual ~Application();

    virtual void InitVariables(){}
    virtual bool Init(const std::string  &title, uint32_t width, uint32_t height);
    virtual bool Update() = 0;
    virtual void Exit();

    virtual void OnWindowClose() = 0;

    virtual std::string GetAbsoluteResourcePath();
    virtual std::string GetAbsoluteWorkingDirectoryPath();
    virtual std::string GetApplicationId() = 0;
    virtual AppContext * Ctx();


    VarGroup & GetEngineVars();
    VarGroup & GetAppVars();

    static void LoadSettings(VarGroup & group, const std::string & settingsFile, Logger * log = nullptr);
};
