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

	m_scenegraph = new sg::sg_scenegraph(this->get_logger(), this->get_timer());
	m_graphics_manager = m_scenegraph->get_graphics_manager();

	auto cam = sg::sg_camera_object_ptr(new sg::sg_camera_object());

    mesh_obj = m_scenegraph->load_mesh_object("res/mrfixit.iqm",true);
    m_scenegraph->add_object(mesh_obj);

    return true;
}

bool MaterialTest::update()
{
    if(wnd->update())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_scenegraph->render_all();

        wnd->swap_buffers();
        return true;
    }
    return false;
}

void MaterialTest::exit()
{
    m_graphics_manager = nullptr;

    delete m_scenegraph;

    application::exit();
}

void MaterialTest::on_window_close()
{

}
