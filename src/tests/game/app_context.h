#pragma once
#include "../../application/window.h"
#include "../../scenegraph/sg_graphics_manager.h"
#include "../../gui/gui_environment.h"
#include "../../scenegraph/scenegraph.h"
#include "physics/physics_manager.h"
struct app_context
{
    window* win;
    sg::sg_graphics_manager_ptr gm;
    gui_environment* env;
    sg::scenegraph* sg;
    physics_manager* pm;
};
