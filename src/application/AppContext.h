#pragma once

#include "Window.h"
#include "opengl/OpenGLUtil.h"
#include "scenegraph/SGGraphicsManager.h"
#include "gui/GUIEnvironment.h"
#include "scenegraph/SGScenegraph.h"
#include "physics/PhysicsManager.h"
#include "network/NetworkManagerWin32.h"
#include "irrklang.h"
#include "utility/logger.h"
#include "resources/SoundManager.h"


struct AppContext
{
    AppContext()
    {
        _window = nullptr;
        _graphicsManager = nullptr;
        _guiEnv = nullptr;
        _scenegraph = nullptr;
        _physicsManager = nullptr;
        _soundManager = nullptr;
        _timer = nullptr;
        _logger = nullptr;
        _glUtil = nullptr;
    }

    bool IsInitialized()
    {
        return _window && _glUtil && _graphicsManager && _scenegraph && _timer && _logger;
    }

    Window* _window;
    OpenGLUtil * _glUtil;
    sg::sg_graphics_manager_ptr _graphicsManager;
    GUIEnvironment* _guiEnv;
    sg::SGScenegraph* _scenegraph;
    PhysicsManager* _physicsManager;
    sound_manager* _soundManager;
    timer_ptr   _timer;
    Logger* _logger;

};
