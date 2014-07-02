#pragma once

#include "../Application/Window.h"
#include "../scenegraph/SGGraphicsManager.h"
#include "../gui/GUIEnvironment.h"
#include "../scenegraph/Scenegraph.h"
#include "physics/PhysicsManager.h"
#include "baigiamasis_quadcopter/GameData.h"
#include "../network/NetworkManagerWin32.h"
#include "irrklang.h"
#include "../utility/Logger.h"
#include "../resources/SoundManager.h"

struct app_context
{
    ~app_context()
    {
        delete env;
        delete sg;
        delete pm;
        delete gd;
        delete nm;
        delete sm;
        delete log;
    };

    Window* win;
    sg::sg_graphics_manager_ptr gm;
    gui_environment* env;
    sg::scenegraph* sg;
    physics_manager* pm;
    game_data* gd;
    network_manager_win32* nm;
    sound_manager* sm;
    logger* log;
};
