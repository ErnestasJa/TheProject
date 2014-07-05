#include "precomp.h"
#include "MaterialTest.h"
#include "application/window.h"
#include "scenegraph/sg_mesh_object.h"

MaterialTest::MaterialTest(uint32_t argc, const char ** argv): application(argc,argv)
{

}

MaterialTest::~MaterialTest()
{

}

sg::sg_mesh_object_ptr mesh_obj;

bool MaterialTest::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.2,1,0.2,0);

    auto sg = m_app_context->scenegraph;
	auto cam = sg::sg_camera_object_ptr(new sg::sg_camera_object(m_app_context,glm::vec3(0,0,-10),glm::vec3(0,0,10),glm::vec3(0,1,0)));

	mesh_obj = sg->load_mesh_object("res/mrfixit.iqm",true);
    sg->add_object(cam);
    sg->add_object(mesh_obj);

    return true;
}

bool MaterialTest::update()
{
    if(m_app_context->app_window->update() && !m_app_context->app_window->getShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_app_context->scenegraph->render_all();

        m_app_context->app_window->swap_buffers();
        return true;
    }
    return false;
}

void MaterialTest::exit()
{
    application::exit();
}

void MaterialTest::on_window_close()
{

}
