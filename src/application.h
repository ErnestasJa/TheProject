#pragma once

#include "precomp.h"
#include "physfs.h"
#include "logger.h"
class window;
class opengl_util;
class timer;

class application
{
protected:
    window * wnd;
    opengl_util * gl_util;
    timer * main_timer;
    logger * _log;

    int32_t argc;
    const char ** argv;

public:

    application(int32_t argc, const char ** argv);
    virtual ~application();

    virtual bool init(const std::string  &title, uint32_t width, uint32_t height);
    virtual bool update() = 0;
    virtual void exit();
    virtual logger *getLogger();
    virtual timer *getTimer();
};
