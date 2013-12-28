#pragma once

#include "application/application.h"
#include "gui/gui_event_listener.h"

class iqmesh;
class texture;
class frame_buffer_object;
class shader;
class quad;
class image_loader;
class iqmloader;
struct mesh;
class test_application: public application
{
protected:
    std::vector<std::shared_ptr<mesh> > * mesh_cache;
    std::vector<std::shared_ptr<shader> > * shader_cache;
    std::vector<std::shared_ptr<texture> > * tex_cache;
    std::vector<std::shared_ptr<frame_buffer_object> > * fbo_cache;

    image_loader * img_loader;
    iqmloader * iqm_loader;

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

public:
    test_application(uint32_t argc, const char ** argv);
    ~test_application();

    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();
    void show_fps();

protected:
    void init_gl();
};
