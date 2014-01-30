#ifndef TEST_GAME_H
#define TEST_GAME_H

#include "application/application.h"

namespace sg
{
class scenegraph;
struct sg_material;
}

struct shader;
class shader_loader;
class texture;
class graphics_manager;
class mesh_loader;
struct mesh;

///physics predeclares
class PhysicsManager;

class test_game: public application
{
protected:
    graphics_manager*   m_graphics_manager;
    sg::scenegraph  *   m_scenegraph;
    PhysicsManager  *   m_physics_manager;

    uint32_t m_last_time, m_current_time;

public:
    test_game(uint32_t argc, const char ** argv);
    virtual ~test_game();

    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();

    bool init_scene();
    bool init_physics();
    void cam_move();
};

#endif // TEST_GAME_H
