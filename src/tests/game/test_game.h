#ifndef TEST_GAME_H
#define TEST_GAME_H

#include "application/application.h"

namespace sg
{
class scenegraph;
struct sg_material;
class sg_graphics_manager;
typedef std::shared_ptr<sg_graphics_manager> sg_graphics_manager_ptr;
}

struct shader;
class shader_loader;
class texture;

class mesh_loader;
struct mesh;

///physics predeclares
class physics_manager;

class test_game: public application
{
protected:
    sg::sg_graphics_manager_ptr m_graphics_manager;
    sg::scenegraph*             m_scenegraph;
    physics_manager*            m_physics_manager;

    uint32_t m_last_time, m_current_time;
    glm::ivec2 m_last_mouse_pos, m_current_mouse_pos, m_window_size;
    glm::vec3 m_cam_rot;

public:
    test_game(uint32_t argc, const char ** argv);
    virtual ~test_game();

    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();

    bool init_scene();
    bool init_physics();
    void cam_move();

public:
    void on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier);
    void on_mouse_move(double x, double y);
};

#endif // TEST_GAME_H
