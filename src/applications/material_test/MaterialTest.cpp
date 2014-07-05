#include "precomp.h"
#include "MaterialTest.h"
#include "application/window.h"
#include "scenegraph/SGMeshObject.h"

MaterialTest::MaterialTest(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

MaterialTest::~MaterialTest()
{

}

sg::sg_mesh_object_ptr mesh_obj;

bool MaterialTest::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClearColor(0.2,1,0.2,0);

    auto sg = _appContext->_scenegraph;
	auto cam = sg::sg_camera_object_ptr(new sg::sg_camera_object(_appContext,glm::vec3(0,0,-10),glm::vec3(0,0,10),glm::vec3(0,1,0)));

	mesh_obj = sg->load_mesh_object("res/mrfixit.iqm",true);
    sg->add_object(cam);
    sg->add_object(mesh_obj);

    return true;
}

bool MaterialTest::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose())
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        _appContext->_scenegraph->render_all();

        _appContext->_window->SwapBuffers();
        return true;
    }
    return false;
}

void MaterialTest::Exit()
{
    Application::Exit();
}

void MaterialTest::OnWindowClose()
{

}
