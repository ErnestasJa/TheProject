#pragma once

#include "application/application.h"
#include "gui/gui_event_listener.h"

class iqmesh;
class texture;
class frame_buffer_object;
class shader;
class quad;
class gui_environment;
class gui_element;
class font_renderer;

class gui_and_fonts_application: public application,gui_event_listener
{
protected:
    std::vector<std::shared_ptr<shader> > * shader_cache;
    std::vector<std::shared_ptr<texture> > * tex_cache;
    std::vector<std::shared_ptr<frame_buffer_object> > * fbo_cache;
    uint32_t frame_count;
    uint32_t last_time;

    ///matrices
    glm::mat4 M, V, P, MVP;

    ///these normally should not be in this class and should not be raw pointers, but we want them to be destroyed at exit and not class destructor
    frame_buffer_object * fbo;
    texture             * tex;
    texture             * ztex;
    shader              * qsh;
    shader              * gqsh;
    shader              * sh;
    quad                * q;
    quad                * q2;

    gui_environment * env;
    gui_element *test;
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
