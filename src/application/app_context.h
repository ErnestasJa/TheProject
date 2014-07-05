#pragma once

#include "../application/window.h"
#include "../scenegraph/sg_graphics_manager.h"
#include "../gui/gui_environment.h"
#include "../scenegraph/sg_scenegraph.h"
#include "physics/physics_manager.h"
#include "../network/network_manager_win32.h"
#include "irrklang.h"
#include "../utility/logger.h"
#include "../resources/sound_manager.h"

struct app_context
{
    app_context()
    {
        app_window = nullptr;
        graphics_manager = nullptr;
        gui_env = nullptr;
        scenegraph = nullptr;
        physics_mgr = nullptr;
        sound_mgr = nullptr;
        app_timer = nullptr;
        log = nullptr;
    }

    virtual ~app_context()
    {

    };

    window* app_window;
    sg::sg_graphics_manager_ptr graphics_manager;
    gui_environment* gui_env;
    sg::sg_scenegraph* scenegraph;
    physics_manager* physics_mgr;
    sound_manager* sound_mgr;
    timer_ptr   app_timer;
    logger* log;
};
