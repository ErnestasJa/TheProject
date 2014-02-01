#include "precomp.h"
#include "test_game.h"

#include "application/window.h"

#include "resources/resource_cache.h"
#include "opengl/texture.h"
#include "utility/timer.h"
#include "utility/logger.h"
#include "opengl/opengl_util.h"
#include "opengl/mesh.h"
#include "opengl/shader.h"
#include "opengl/graphics_manager.h"

#include "resources/mesh_loader.h"
#include "resources/shader_loader.h"
#include "resources/image_loader.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/sg_objects.h"
#include "physics/Physics.h"

test_game::test_game(uint32_t argc, const char ** argv): application(argc,argv)
{
    //ctor
}

test_game::~test_game()
{
    //dtor
}

bool test_game::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);
    wnd->sig_key_event().connect(sigc::mem_fun(this,&test_game::on_key_event));
    wnd->sig_mouse_moved().connect(sigc::mem_fun(this,&test_game::on_mouse_move));


    m_graphics_manager = new graphics_manager(this->get_logger());
    m_scenegraph = new sg::scenegraph();

    m_physics_manager = new physics_manager(btVector3(0,-9.83f,0));

    if(!init_scene())
        return false;

    main_timer->tick();

    m_current_time = m_last_time = main_timer->get_real_time();

    return !this->gl_util->check_and_output_errors();
}

bool test_game::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        main_timer->tick();
        m_last_time = m_current_time;
        m_current_time = main_timer->get_real_time();
        float delta_time = ((float)(m_current_time - m_last_time)) / 1000.0f;

        cam_move();

        m_physics_manager->update(delta_time);
        m_scenegraph->render_all();

        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;

}

void test_game::exit()
{
    delete m_graphics_manager;

    delete m_scenegraph;
    delete m_physics_manager;

    application::exit();
}

bool test_game::init_scene()
{
    sg::sg_mesh_object_ptr obj;
    shader_ptr sh;
    mesh_ptr m;

    ///load model
    m=m_graphics_manager->get_mesh_loader()->load("res/wood_tower.iqm");
    if(!m) return false;
    m->init();

    obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m));
    sh = m_graphics_manager->get_shader_loader()->load("res/static_mesh");

    if(!sh) return false;
    {
        sg::sg_material & mat = obj->get_material(0);
        mat.mat_shader = sh;
        mat.mat_textures[0] = m_graphics_manager->load_texture("res/wood_tower_color.png");
    }

    obj->get_transform() = glm::translate(glm::mat4(),glm::vec3(0,5.25,-5));
    m_scenegraph->add_object(obj);
    m_physics_manager->create_trimesh_body(obj,btVector3(1,1,1));

    {
        sg::sg_aabb_wireframe_object_ptr wireframe = share(new sg::sg_aabb_wireframe_object(obj));
        sg::sg_material & wmat = wireframe->get_material(0);
        wmat.mat_shader = m_graphics_manager->get_shader_loader()->load("res/line");
        m_scenegraph->add_object(wireframe);
    }
    ///done loading

    loopi(5)
    {
        ///load model
        m=m_graphics_manager->get_mesh_loader()->load("res/trashcan.iqm");
        if(!m) return false;
        m->init();

        obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m));
        sh = m_graphics_manager->get_shader_loader()->load("res/static_mesh");

        if(!sh) return false;
        {
            sg::sg_material & mat = obj->get_material(0);
            mat.mat_shader = sh;
            mat.mat_textures[0] = m_graphics_manager->load_texture("res/trashcan_diffuse.png");
        }

        obj->get_transform() = glm::translate(glm::mat4(),glm::vec3(i*2,15,-7));
        m_scenegraph->add_object(obj);
        m_physics_manager->create_box(obj,10.0f);

        {
            sg::sg_aabb_wireframe_object_ptr wireframe = share(new sg::sg_aabb_wireframe_object(obj));
            sg::sg_material & wmat = wireframe->get_material(0);
            wmat.mat_shader = m_graphics_manager->get_shader_loader()->load("res/line");
            m_scenegraph->add_object(wireframe);
        }
        ///done loading
    }

    ///load map
    sg::sg_material lvl_mat;
    lvl_mat.mat_shader = m_graphics_manager->get_shader_loader()->load("res/static_mesh");

    obj=m_graphics_manager->load_mesh_object("res/maps/nuke/nuke.iqm",lvl_mat,true);
    obj->get_transform() = glm::scale(obj->get_transform(),glm::vec3(1,1,1));

    m_scenegraph->add_object(obj);
    m_physics_manager->create_trimesh_body(obj,btVector3(1,1,1));
    ///done loading

    sg::sg_camera_object_ptr cam = sg::sg_camera_object_ptr(new sg::sg_camera_object(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0)));

    m_scenegraph->set_active_camera(cam);

    wnd->set_mouse_pos(m_current_mouse_pos = m_last_mouse_pos = wnd->get_window_size()/2);
    wnd->set_cursor_disabled(true);

    return true;
}

bool test_game::init_physics()
{
    return true;
}

void test_game::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{
    if(sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera())
    {
            cam->key_pressed(key, scan_code, action, modifier);
    }
}

void test_game::on_mouse_move(double x, double y)
{
    m_last_mouse_pos = m_current_mouse_pos;
    m_current_mouse_pos = glm::ivec2(x,y);
    glm::ivec2 delta_pos =  m_current_mouse_pos - m_last_mouse_pos;

    if(sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera())
    {
        cam->mouse_moved(-delta_pos.x/100.0f,-delta_pos.y/100.0f);
    }
}

void test_game::cam_move()
{

}
