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
class menu_state:public game_state,public gui_event_listener
{
private:
    gui_edit_box* eb;
    gui_button* btn;
    gui_window* win;
    gui_image* img;
    gui_static_text* st;
    gui_environment* m_env;
    gui_pane* pane;


    toggle_group* quads;
    toggle_group* levels;

    sg::sg_mesh_object_ptr obj;
    sg::sg_material_static_mesh * sm_mat;
    sg::sg_skybox_object_ptr skybox;


    std::vector<gui_element*> elems;

    float m_rot;
public:
    menu_state(state_manager* sm);

    virtual ~menu_state()
    {
    }
    void on_load();

    void create_scene();

    void create_quad_selections();

    void create_level_selections();

    void create_start_window();

    void on_unload();

    void start();

    void update(float delta);

    bool on_event(const gui_event &e);

    void render();
protected:
};
