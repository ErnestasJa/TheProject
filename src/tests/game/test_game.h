#ifndef TEST_GAME_H
#define TEST_GAME_H

#include "application/application.h"

namespace sg
{
class scenegraph;
class sg_quad;
typedef std::shared_ptr<sg_quad> sg_quad_ptr;
struct sg_material;
typedef std::shared_ptr<sg_material> sg_material_ptr;
class sg_graphics_manager;
typedef std::shared_ptr<sg_graphics_manager> sg_graphics_manager_ptr;
}

struct shader;
typedef std::shared_ptr<shader> shader_ptr;
class shader_loader;
class texture;
typedef std::shared_ptr<texture> texture_ptr;
class frame_buffer_object;
typedef std::shared_ptr<frame_buffer_object> frame_buffer_object_ptr;

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

    ///shadows
    frame_buffer_object_ptr m_shadow_fbo, m_shadow_filter;
    texture_ptr m_shadow_tex;
    sg::sg_material_ptr m_mat_gauss_v, m_mat_gauss_h, m_mat_first_pass, m_mat_final_pass, m_mat_static_mesh;
    sg::sg_quad_ptr m_quad;


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
