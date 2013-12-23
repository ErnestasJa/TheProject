#pragma once

#include "application.h"
#include "resource_cache.h"
#include "iqmesh.h"
#include "texture.h"
#include "frame_buffer_object.h"
#include "shader.h"
#include "quad.h"
#include "gui_event_listener.h"

class gui_environment;
class gui_element;
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
    shader              * sh;
    quad                * q;

    gui_environment * env;
    gui_element *test;
public:
    gui_and_fonts_application(uint32_t argc, const char ** argv);
    ~gui_and_fonts_application();
    bool init(const std::string & title, uint32_t width, uint32_t height);
    void on_event(gui_event e);
    virtual bool update();
    void exit();
    void show_fps();
};