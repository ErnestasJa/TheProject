#pragma once

class window;
class opengl_util;
class timer; typedef std::shared_ptr<timer> timer_ptr;
class logger;

class application
{
protected:
    window * wnd;
    opengl_util * gl_util;
    timer_ptr main_timer;
    logger * m_log;

    int32_t argc;
    const char ** argv;
    virtual void output_versions();
public:

    application(int32_t argc, const char ** argv);
    virtual ~application();

    virtual bool init(const std::string  &title, uint32_t width, uint32_t height);
    virtual bool update() = 0;
    virtual void exit();
    virtual logger * get_logger();
    virtual timer_ptr get_timer();
};
