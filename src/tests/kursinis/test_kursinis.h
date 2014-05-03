#pragma once

#include "application/application.h"
#include "Objektas.h"
#include "gui/gui_event_listener.h"

namespace sg
{
class scenegraph;
class sg_quad;
class isg_object;
typedef std::shared_ptr<sg_quad> sg_quad_ptr;
typedef std::shared_ptr<isg_object> sg_object_ptr;
struct sg_material;
typedef std::shared_ptr<sg_material> sg_material_ptr;
class sg_graphics_manager;
typedef std::shared_ptr<sg_graphics_manager> sg_graphics_manager_ptr;
}


class gui_environment;
class test_kursinis: public application, public gui_event_listener
{
protected:
    sg::sg_graphics_manager_ptr m_graphics_manager;
    sg::scenegraph*             m_scenegraph;
    gui_environment* env;

    ///select obj
    sg::sg_object_ptr selected_obj;
    glm::vec2 mouse_start;
    glm::vec3 obj_start;

    ///3 kunai
    Objektas * obj[3];
    bool simuliuoti;
public:
    test_kursinis(uint32_t argc, const char ** argv);
    ~test_kursinis();

    bool init_scene();

    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();

    void on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier);
    void on_mouse_key_event(int32_t button, int32_t action, int32_t mod);
    void on_mouse_move(double x, double y);
    void on_resize(int32_t x, int32_t y);
    virtual void on_event(gui_event e);
};
