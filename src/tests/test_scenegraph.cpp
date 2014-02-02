#include "precomp.h"
#include "test_scenegraph.h"

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

test_scenegraph::test_scenegraph(uint32_t argc, const char ** argv): application(argc,argv)
{

}

test_scenegraph::~test_scenegraph()
{
    //dtor
}


bool test_scenegraph::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);

    m_graphics_manager = new graphics_manager(this->get_logger());
    m_shader_loader = new shader_loader(this->get_logger());
    m_mesh_loader = new mesh_loader(this->get_logger());
    m_scenegraph = new sg::scenegraph(this->get_timer());

    if(!init_scene())
        return false;

    return !this->gl_util->check_and_output_errors();
}

bool test_scenegraph::init_scene()
{
    sg::sg_mesh_object_ptr obj;
    shader_ptr sh;
    mesh_ptr m;

    ///load model
    m=m_mesh_loader->load("res/wood_tower.iqm");
    if(!m) return false;
    m->init();

    obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m));
    sh = m_shader_loader->load("res/static_mesh");

    if(!sh) return false;
    {
        sg::sg_material & mat = obj->get_material(0);
        mat.mat_shader = sh;
        mat.mat_textures[0] = m_graphics_manager->load_texture("res/wood_tower_color.png");
    }

    m_scenegraph->add_object(obj);
    ///done loading

    ///load model
    m=m_mesh_loader->load("res/trashcan.iqm");
    if(!m) return false;
    m->init();

    obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m));
    sh = m_shader_loader->load("res/static_mesh");

    if(!sh) return false;
    {
        sg::sg_material & mat = obj->get_material(0);
        mat.mat_shader = sh;
        mat.mat_textures[0] = m_graphics_manager->load_texture("res/trashcan_diffuse.png");
    }

    obj->get_transform() = glm::translate(glm::mat4(),glm::vec3(0,0,5));
    m_scenegraph->add_object(obj);
    ///done loading

    ///load model
    m=m_mesh_loader->load("res/terrain.iqm");
    if(!m) return false;
    m->init();

    obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m));
    sh = m_shader_loader->load("res/static_mesh");

    if(!sh) return false;
    {
        sg::sg_material & mat = obj->get_material(0);
        mat.mat_shader = sh;
        mat.mat_textures[0] = m_graphics_manager->load_texture("res/terrain.png");
    }

    obj->get_transform() = glm::translate(glm::mat4(),glm::vec3(0,0,5));
    obj->get_transform() = glm::scale(obj->get_transform(),glm::vec3(0.2,0.2,0.2));

    m_scenegraph->add_object(obj);
    ///done loading

    sg::sg_camera_object_ptr cam = sg::sg_camera_object_ptr(new sg::sg_camera_object(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0)));

    m_scenegraph->set_active_camera(cam);

    return true;
}

bool test_scenegraph::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        main_timer->tick();

        cam_move();

        m_scenegraph->render_all();

        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;
}

void test_scenegraph::cam_move()
{
    if(sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera())
    {

        if(wnd->get_key(GLFW_KEY_W))
        {
            cam->get_transform() = glm::translate(glm::mat4(),glm::vec3(0,0,2)) * cam->get_transform();
        }
        else if(wnd->get_key(GLFW_KEY_S))
        {
            cam->get_transform() = glm::translate(glm::mat4(),glm::vec3(0,0,-2)) * cam->get_transform();
        }
        else if(wnd->get_key(GLFW_KEY_A))
        {
            cam->get_transform() = glm::translate(glm::mat4(),glm::vec3(2,0,0)) * cam->get_transform();
        }
        else if(wnd->get_key(GLFW_KEY_D))
        {
            cam->get_transform() = glm::translate(glm::mat4(),glm::vec3(-2,0,0)) * cam->get_transform();
        }

        if(wnd->get_key(GLFW_KEY_LEFT))
        {
            cam->get_transform() = glm::rotate<float>(glm::mat4(),-2,glm::vec3(0,1,0)) * cam->get_transform();
        }
        else if(wnd->get_key(GLFW_KEY_RIGHT))
        {
            cam->get_transform() = glm::rotate<float>(glm::mat4(),2,glm::vec3(0,1,0)) * cam->get_transform();
        }
        else if(wnd->get_key(GLFW_KEY_UP))
        {
            cam->get_transform() = glm::rotate<float>(glm::mat4(),-2,glm::vec3(1,0,0)) * cam->get_transform();
        }
        else if(wnd->get_key(GLFW_KEY_DOWN))
        {
            cam->get_transform() = glm::rotate<float>(glm::mat4(),2,glm::vec3(1,0,0)) * cam->get_transform();
        }
    }
}


void test_scenegraph::exit()
{
    delete m_graphics_manager;
    delete m_mesh_loader;
    delete m_shader_loader;

    application::exit();
}
