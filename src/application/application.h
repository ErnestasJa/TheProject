#pragma once

class Window;
class opengl_util;
class timer; typedef std::shared_ptr<timer> timer_ptr;
class logger;

class Application
{
protected:
    Window * wnd;
    opengl_util * gl_util;
    timer_ptr main_timer;
    logger * m_log;

    int32_t argc;
    const char ** argv;
    virtual void output_versions();
public:

    Application(int32_t argc, const char ** argv);
    virtual ~Application();

    virtual bool init(const std::string  &title, uint32_t width, uint32_t height);
    virtual bool update() = 0;
    virtual void exit();

    virtual void on_window_close() = 0;

    virtual logger * get_logger();
    virtual timer_ptr get_timer();
};
