#pragma once

class Window;
class OpenGLUtil;
class Logger;

#include "AppContext.h"

class Application
{
protected:
    std::string _resourcePath;
    std::string _workingDirectoryPath;
    AppContext * _appContext;
    int32_t _argc;
    const char ** _argv;
    virtual void OutputPhysFSVersions();
public:

    Application(int32_t argc, const char ** argv);
    virtual ~Application();

    virtual bool Init(const std::string  &title, uint32_t width, uint32_t height);
    virtual bool Update() = 0;
    virtual void Exit();

    virtual void OnWindowClose() = 0;

    virtual std::string GetAbsoluteResourcePath();
    virtual std::string GetAbsoluteWorkingDirectoryPath();
    virtual AppContext * Ctx();
};
