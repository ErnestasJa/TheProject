#pragma once
#include "../app_context.h"
#include "game_state.h"

class gui_edit_box;
class gui_button;
class gui_window;
class gui_image;
class gui_static_text;
class gui_environment;
class gui_pane;
class toggle_group;
namespace sg{
class quadcopter;
}

class play_state:public game_state,public gui_event_listener
{
private:
    float m_rot;
    gui_environment* m_env;
    sg::quadcopter* m_quadcopter;
public:
    play_state(state_manager* sm);

    virtual ~play_state()
    {
    }
    void on_load();

    void on_unload();

    void start();

    void update(float delta);

    bool on_event(const gui_event &e);

    void render();
protected:
};
