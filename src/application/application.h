#pragma once

class Window;
class OpenGLUtil;
class Timer; typedef std::shared_ptr<Timer> timer_ptr;
class logger;

class Application
{
protected:
    Window * _window;
    OpenGLUtil * _GLUtil;
    timer_ptr _mainTimer;
    logger * _logger;

    int32_t _argc;
    const char ** _argv;
    virtual void OutputVersions();
public:

    Application(int32_t argc, const char ** argv);
    virtual ~Application();

    virtual bool Init(const std::string  &title, uint32_t width, uint32_t height);
    virtual bool Update() = 0;
    virtual void Exit();

    virtual void OnWindowClose() = 0;

    virtual logger * GetLogger();
    virtual timer_ptr GetTimer();
};
