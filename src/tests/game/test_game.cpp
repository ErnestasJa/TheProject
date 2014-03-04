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


#include "resources/mesh_loader.h"
#include "resources/shader_loader.h"
#include "resources/image_loader.h"
#include "scenegraph/sg_graphics_manager.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/sg_scenegraph_loader.h"
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

    m_scenegraph = new sg::scenegraph(this->get_logger(),this->get_timer());
    m_graphics_manager = m_scenegraph->get_graphics_manager();

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
        m_scenegraph->update(delta_time);
        m_scenegraph->render_all();

        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;

}

void test_game::exit()
{
    m_graphics_manager = nullptr;

    delete m_scenegraph;
    delete m_physics_manager;

    application::exit();
}

bool test_game::init_scene()
{
    sg::sg_mesh_object_ptr obj;
    mesh_ptr m;

    ///load model
    m=m_graphics_manager->get_mesh_loader()->load("res/wood_tower.iqm");

    if(!m)
        return false;

    obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_scenegraph,m));

    sg::sg_material_static_mesh * sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
    sm_mat->mat_texture= m_graphics_manager->load_texture("res/wood_tower_color.png");


    obj->set_position(glm::vec3(0,5.25,-5));
    m_scenegraph->add_child(obj);
    m_physics_manager->create_trimesh_body(obj,btVector3(1,1,1));
    ///done loading

    /*for(uint32_t i=0; i<5; i++)
    {
        ///load trashcan
        m=m_graphics_manager->get_mesh_loader()->load("res/trashcan.iqm");

        if(!m)
            return false;

        obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_scenegraph,m));

        sg::sg_material_static_mesh * sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
        sm_mat->mat_texture= m_graphics_manager->load_texture("res/no_tex.png");

        obj->set_position(glm::vec3(i*2,15,-7));
        m_scenegraph->add_child(obj);
        m_physics_manager->create_box(obj,10.0f);
        ///done loading
    }*/

    ///add light
    sg::sg_light_object_ptr lobj = m_scenegraph->add_light_object();
    lobj->set_position(glm::vec3(0,20,100));
    ///done loading

   /* ///load sprite
    sg::sg_sprite_ptr sobj = sg::sg_sprite_ptr(new sg::sg_sprite(m_scenegraph));

    sg::sg_material_point_sprite* psm_mat = static_cast<sg::sg_material_point_sprite*>(sobj->get_material(0).get());
    psm_mat->mat_texture= m_graphics_manager->load_texture("res/light.png");

    lobj->add_child(sobj);
    ///done loading

    ///load map
    obj=m_scenegraph->load_mesh_object("res/maps/nuke/nuke.iqm",true);

    m_scenegraph->add_child(obj);
    m_physics_manager->create_trimesh_body(obj,btVector3(1,1,1));
    ///done loading*/

    sg::sg_scenegraph_loader sg_loader;
    sg_loader.load_scene(m_scenegraph,"test_scene.ibs");

    sg::sg_camera_object_ptr cam = sg::sg_camera_object_ptr(new sg::sg_camera_object(m_scenegraph,glm::vec3(0,5,20),glm::vec3(0,0,0),glm::vec3(0,1,0)));

    m_scenegraph->set_active_camera(cam);
    m_scenegraph->add_child(cam);

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
        if(action==GLFW_PRESS || action == GLFW_REPEAT )
        {
            if(key==GLFW_KEY_W)
                cam->walk(1);
            if(key==GLFW_KEY_S)
                cam->walk(-1);
            if(key==GLFW_KEY_A)
                cam->strafe(-1);
            if(key==GLFW_KEY_D)
                cam->strafe(1);
        }

        if(action==GLFW_PRESS)
        {
            if(key==GLFW_KEY_SPACE)
            {
                glm::vec3 pos   = cam->get_position();
                glm::vec3 look  = glm::normalize(cam->get_look());

                ///load model
                mesh_ptr m=m_graphics_manager->get_mesh_loader()->load("res/trashcan.iqm");
                if(!m) return;

                sg::sg_mesh_object_ptr obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_scenegraph,m));
                obj->set_position(pos);

                sg::sg_material_static_mesh * sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
                sm_mat->mat_texture= m_graphics_manager->load_texture("res/no_tex.png");

                m_scenegraph->add_child(obj);

                btRigidBody * body = m_physics_manager->create_box(obj,10.0f);
                body->setLinearVelocity(m_physics_manager->glm_to_bt(look*10.0f));
            }

        }
    }
}

void test_game::on_mouse_move(double x, double y)
{
    m_last_mouse_pos = m_current_mouse_pos;
    m_current_mouse_pos = glm::ivec2(x,y);
    glm::ivec2 delta_pos =  m_current_mouse_pos - m_last_mouse_pos;

    if(sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera())
    {
        if(delta_pos.x!=0 || delta_pos.y!=0)
        {
            glm::quat r = cam->get_rotation();

            glm::quat rot_x(glm::vec3(0,-delta_pos.x/100.0f,0)), rot_y(glm::vec3(delta_pos.y/100.0f,0,0));

            r= rot_x * r;
            r= r * rot_y;

            cam->set_rotation(r);
        }
    }
}

void test_game::cam_move()
{

}
