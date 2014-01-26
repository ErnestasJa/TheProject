#pragma once

#include "application/application.h"
#include "gui/gui_event_listener.h"

class frame_buffer_object;
class gui_environment;
class font_renderer;
class gui_static_text;
class gui_and_fonts_application: public application,gui_event_listener
{
protected:
    uint32_t frame_count;
    uint32_t last_time;

    ///matrices
    glm::mat4 M, V, P, MVP;

    gui_environment * env;
    gui_static_text * test1;
    gui_static_text * test2;

    font_renderer *renderer;
public:
    gui_and_fonts_application(uint32_t argc, const char ** argv);
    ~gui_and_fonts_application();
    bool init(const std::string & title, uint32_t width, uint32_t height);
    void on_event(gui_event e);
    virtual bool update();
    void exit();
    void show_fps();
};
