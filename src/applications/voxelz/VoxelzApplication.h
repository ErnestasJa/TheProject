#ifndef TEST_GAME_H
#define TEST_GAME_H

#include "Application/Application.h"
#include "LinearMath/btVector3.h"
#include "gui/GUIEventListener.h"

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
class btRigidBody;
class gui_environment;
class state_manager;
struct AppContext;
class voxelz_Application: public Application,public gui_event_listener
{
protected:
    sg::sg_graphics_manager_ptr _graphicsManager;
    sg::SGScenegraph*             m_scenegraph;
    physics_manager*            m_physics_manager;
    state_manager* m_state_manager;

    uint32_t m_last_time, m_current_time;
    glm::ivec2 m_last_mouse_pos, m_current_mouse_pos, m_window_size;

    gui_environment *env;
    AppContext *ctx;
public:
    voxelz_Application(uint32_t argc, const char ** argv);
    virtual ~voxelz_Application();

    bool Init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool Update();
    void Exit();
public:
    void on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier);
    void on_mouse_move(double x, double y);
    virtual bool on_event(const gui_event & e);
};

#endif // TEST_GAME_H

