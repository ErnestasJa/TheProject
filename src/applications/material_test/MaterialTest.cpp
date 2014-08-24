#include "precomp.h"
#include "MaterialTest.h"
#include "application/window.h"
#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "resources/ShaderLoader.h"
#include "scenegraph/Camera.h"
#include "Voxel/Chunk.h"


MaterialTest::MaterialTest(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

MaterialTest::~MaterialTest()
{

}

MeshPtr mesh;
ShaderPtr sh;
CameraPtr cam;
Chunk *chk;

void InitPlaneMesh(AppContext * ctx)
{
    BufferObject<glm::vec3> * pos = new BufferObject<glm::vec3>();
    pos->data.push_back(glm::vec3(-0.5, 0.5, 0));
    pos->data.push_back(glm::vec3( 0.5, 0.5, 0));
    pos->data.push_back(glm::vec3( -0.5,-0.5, 0));
    pos->data.push_back(glm::vec3(0.5,-0.5, 0));

    IndexBufferObject<uint32_t> * id = new IndexBufferObject<uint32_t>();
    id->data.push_back(0);
    id->data.push_back(1);
    id->data.push_back(2);
    id->data.push_back(2);
    id->data.push_back(1);
    id->data.push_back(3);

    mesh = share(new Mesh());
    mesh->buffers[Mesh::POSITION] = pos;
    mesh->buffers[Mesh::INDICES] = id;
    mesh->Init();

    sh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/solid_unlit");
    cam=share(new Camera(ctx,glm::vec3(0,0,-5),glm::vec3(0,0,5),glm::vec3(0,1,0)));

    chk=new Chunk();
    chk->SetupSphere();
    chk->Generate();
}

bool MaterialTest::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    _appContext->_window->SigKeyEvent().connect(sigc::mem_fun(this,&MaterialTest::OnKeyEvent));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
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

        cam->Update(0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP   = cam->GetViewProjMat() * Model;

        MVar<glm::mat4>(0, "mvp", MVP).Set();
        sh->Set();
        mesh->Render();
        chk->Render();

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
    if(key==GLFW_KEY_W)
        cam->Walk(1);
    if(key==GLFW_KEY_S)
        cam->Walk(-1);
    if(key==GLFW_KEY_A)
        cam->Strafe(-1);
    if(key==GLFW_KEY_D)
        cam->Strafe(1);
    if(key==GLFW_KEY_SPACE)
    {
        float ww,wh;
        ww=_appContext->_window->GetWindowSize().x;
        wh=_appContext->_window->GetWindowSize().y;
        float depth;
        glReadPixels(ww / 2, wh / 2, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);

        glm::vec4 viewport = glm::vec4(0, 0, ww, wh);
        glm::vec3 wincoord = glm::vec3(ww / 2, wh / 2, depth);
        glm::vec3 objcoord = glm::unProject(wincoord, cam->GetViewMat(), cam->GetProjectionMat(), viewport);

        int x = floorf(objcoord.x);
        int y = floorf(objcoord.y);
        int z = floorf(objcoord.z);
        printf("Voxel pos: %f %f %f\n",x,y,z);
        chk->Set(0,0,0,EBT_SAND,false);
        chk->Set(1,0,0,EBT_SAND,false);
        chk->Set(0,1,0,EBT_SAND,false);
        chk->Set(1,1,0,EBT_SAND,false);
    }
}
