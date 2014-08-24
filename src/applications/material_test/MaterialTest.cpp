#include "precomp.h"
#include "MaterialTest.h"
#include "application/window.h"
#include "opengl/Mesh.h"
#include "opengl/shader.h"
#include "opengl/SGMVar.h"
#include "resources/ShaderLoader.h"


MaterialTest::MaterialTest(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

MaterialTest::~MaterialTest()
{

}

MeshPtr mesh;
shader_ptr sh;

void InitPlaneMesh(AppContext * ctx)
{
    BufferObject<glm::vec3> * pos = new BufferObject<glm::vec3>();
    pos->data.push_back(glm::vec3(-0.5, 0.5, 0));
    pos->data.push_back(glm::vec3( 0.5, 0.5, 0));
    pos->data.push_back(glm::vec3( 0.5,-0.5, 0));
    pos->data.push_back(glm::vec3(-0.5,-0.5, 0));

    IndexBufferObject<uint32_t> * id = new IndexBufferObject<uint32_t>();
    id->data.push_back(0);
    id->data.push_back(2);
    id->data.push_back(1);
    id->data.push_back(1);
    id->data.push_back(2);
    id->data.push_back(3);

    mesh = share(new Mesh());
    mesh->buffers[Mesh::POSITION] = pos;
    mesh->buffers[Mesh::INDICES] = id;
    mesh->Init();

    sh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/solid_unlit");
}

bool MaterialTest::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    _appContext->_window->SigKeyEvent().connect(sigc::mem_fun(this,&MaterialTest::OnKeyEvent));

    glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glClearColor(0.2,1,0.2,0);

    InitPlaneMesh(_appContext);

    _appContext->_timer->tick();
    return true;
}

bool MaterialTest::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        _appContext->_timer->tick();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 Projection = glm::perspective(45.0f, 4.0f / 3.0f, 0.1f, 100.0f);

        glm::mat4 View = glm::lookAt(
            glm::vec3(4,3,3),
            glm::vec3(0,0,0),
            glm::vec3(0,1,0)
        );

        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP   = Projection * View * Model;

        sg_mvar<glm::mat4>(0, "mvp", MVP).set();
        sh->set();
        mesh->Render();

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

void MaterialTest::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{

}
