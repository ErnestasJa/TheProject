#pragma once

#include "precomp.h"
#include "physfs.h"

class window;
class opengl_util;
class timer;

class application
{
protected:
    window * wnd;
    opengl_util * gl_util;
    timer * main_timer;

    int32_t argc;
    const char ** argv;

public:

    application(int32_t argc, const char ** argv);
    virtual ~application();

    virtual bool init(const std::string  &title, uint32_t width, uint32_t height);
    virtual bool update() = 0;
    virtual void exit();

    timer * get_timer();
};
