#pragma once

#include "Window.h"
#include "opengl/OpenGLUtil.h"
#include "gui/GUIEnvironment.h"
#include "utility/logger.h"
#include "utility/timer.h"

struct AppContext
{
    AppContext()
    {
        _window = nullptr;
        _guiEnv = nullptr;
        _timer = nullptr;
        _logger = nullptr;
        _glUtil = nullptr;
    }

    bool IsInitialized()
    {
        return _window && _glUtil && _timer && _logger;
    }

    Window* _window;
    OpenGLUtil * _glUtil;
    GUIEnvironment* _guiEnv;
    timer_ptr   _timer;
    Logger* _logger;
};
