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

#include "resources/iqmloader.h"
#include "resources/shader_loader.h"
#include "resources/image_loader.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/sg_mesh_object.h"
#include "scenegraph/sg_camera_object.h"

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
    mesh_cache = create_resource_cache<mesh>();
    tex_cache = create_resource_cache<texture>();

    m_image_loader = new image_loader(this->get_logger());
    m_shader_loader = new shader_loader();
    m_iqm_loader = new iqmloader(this->get_logger());

    m_scenegraph = new sg::scenegraph();

    char * buf;

    if(!read("res/mrfixit.iqm",buf))
        return false;

    mesh_ptr m=m_iqm_loader->load(buf);

    if(!m)
        return false;

    m->generate();

    sg::sg_mesh_object_ptr obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m));
    m_scenegraph->add_object(obj);

    shader_ptr sh = m_shader_loader->load("res/static_mesh");

    if(!sh)
        return false;

    image_ptr img = m_image_loader->load("res/body.png");
    texture_ptr tex = share(new texture());
    tex->generate(img);

    sg::sg_material & mat = obj->get_material(0);
    mat.mat_shader = sh;
    mat.mat_textures[0] = tex;
    obj->get_material(1) = obj->get_material(0);

    sg::sg_camera_object_ptr cam = sg::sg_camera_object_ptr(new sg::sg_camera_object(4.f/3.f,45.f,1.0f,2048.f));
    cam->get_transform() = glm::lookAt(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0));
    cam->get_transform() = glm::rotate<float>(cam->get_transform(),-90,glm::vec3(0,1,0));

    m_scenegraph->set_active_camera(cam);

    return !this->gl_util->check_and_output_errors();
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
            cam->get_transform() = glm::translate(cam->get_transform(),glm::vec3(2,0,0));
        }
        else if(wnd->get_key(GLFW_KEY_S))
        {
            cam->get_transform() = glm::translate(cam->get_transform(),glm::vec3(-2,0,0));
        }
        else if(wnd->get_key(GLFW_KEY_A))
        {
            cam->get_transform() = glm::rotate<float>(cam->get_transform(),-5,glm::vec3(0,1,0));
        }
        else if(wnd->get_key(GLFW_KEY_D))
        {
            cam->get_transform() = glm::rotate<float>(cam->get_transform(),5,glm::vec3(0,1,0));
        }
    }
}


void test_scenegraph::exit()
{
    delete m_image_loader;
    delete m_iqm_loader;
    delete m_shader_loader;

    delete tex_cache;
    delete mesh_cache;

    application::exit();
}
