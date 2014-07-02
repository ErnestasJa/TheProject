#pragma once

#include "Application/Application.h"
#include "gui/GUIEventListener.h"

class frame_buffer_object;
class gui_environment;
class font_renderer;
class gui_static_text;
class gui_button;
class gui_and_fonts_Application: public Application,gui_event_listener
{
protected:
    uint32_t frame_count;
    uint32_t last_time;

    ///matrices
    glm::mat4 M, V, P, MVP;

    gui_environment * env;
    gui_static_text * test1;
    gui_button * test2;

    font_renderer *renderer;
    bool running;
public:
    gui_and_fonts_Application(uint32_t argc, const char ** argv);
    ~gui_and_fonts_Application();
    bool Init(const std::string & title, uint32_t width, uint32_t height);
    void on_event(gui_event e);
    virtual bool Update();
    void Exit();
    void show_fps();
};
