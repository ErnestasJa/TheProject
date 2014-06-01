#pragma once
#include "../../application/window.h"
#include "../../scenegraph/sg_graphics_manager.h"
#include "../../gui/gui_environment.h"
#include "../../scenegraph/scenegraph.h"
#include "physics/physics_manager.h"
#include "game_data.h"
#include "../../network/network_manager_win32.h"
struct app_context
{
    window* win;
    sg::sg_graphics_manager_ptr gm;
    gui_environment* env;
    sg::scenegraph* sg;
    physics_manager* pm;
    game_data* gd;
    network_manager_win32* nm;
};
