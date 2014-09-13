#include "precomp.h"
#include "VoxelzProfilingApp.h"
#include "application/window.h"
#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "resources/ShaderLoader.h"
#include "scenegraph/Camera.h"
#include "Opengl/CubeMesh.h"
#include "Opengl/GridMesh.h"
#include "Voxel/Block.h"
#include "Voxel/Chunk.h"
#include "Voxel/ChunkManager.h"
#include "GUI/GUI.h"

VoxelzProfilingApp::VoxelzProfilingApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

VoxelzProfilingApp::~VoxelzProfilingApp()
{

}

static ShaderPtr sh;
static CameraPtr cam;
static ChunkManager *chkmgr;

#define BEGIN_BENCHMARK ctx->_timer->tick();
#define END_BENCHMARK(name) ctx->_timer->tick(); \
                      printf("Benchmarking %s is over. It took: %d ms.\n",(name),ctx->_timer->get_delta_time());

static void AddSingleChunk(AppContext * ctx)
{
    BEGIN_BENCHMARK
    chkmgr->AddChunk(glm::vec3(0,0,0))->Fill();
    END_BENCHMARK("AddSingleChunk")
}

static void AddManyChunks(AppContext * ctx)
{
    BEGIN_BENCHMARK
    for(int x=-8; x<8; x++)
        for(int z=-8; z<8; z++)
            for(int y=0; y<16; y++)
                chkmgr->AddChunk(glm::vec3(x,y,z))->Fill();
    END_BENCHMARK("AddManyChunks")
}

static void SingleChunkRebuild(AppContext * ctx)
{
    BEGIN_BENCHMARK
    chkmgr->GetChunk(glm::vec3(0,0,0))->Rebuild();
    END_BENCHMARK("RebuildSingle")
}

static void AllChunksRebuild(AppContext * ctx)
{
    BEGIN_BENCHMARK
    for(int x=-8; x<8; x++)
        for(int z=-8; z<8; z++)
            for(int y=0; y<16; y++)
                chkmgr->GetChunk(glm::vec3(x,y,z))->Rebuild();
    END_BENCHMARK("RebuildAll")
}

void VoxelzProfilingApp::Benchmark()
{
    AddSingleChunk(_appContext);

    SingleChunkRebuild(_appContext);

    AddManyChunks(_appContext);

    AllChunksRebuild(_appContext);

}

bool VoxelzProfilingApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    _appContext->_window->SigKeyEvent().connect(sigc::mem_fun(this,&VoxelzProfilingApp::OnKeyEvent));
    _appContext->_window->SigMouseKey().connect(sigc::mem_fun(this,&VoxelzProfilingApp::OnMouseKey));
    _appContext->_window->SigMouseMoved().connect(sigc::mem_fun(this,&VoxelzProfilingApp::OnMouseMove));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.5,0.5,0.7,0);

    sh = (new shader_loader(_appContext->_logger))->load("res/engine/shaders/solid_unlit");
    cam = share(new Camera(_appContext,glm::vec3(0,128,128),glm::vec3(0,0,0),glm::vec3(0,1,0)));

    chkmgr=new ChunkManager();
    printf("\nPROFILE ME TIMBERS!\n");
    Benchmark();
    printf("TIMBERRRRR!\n");
    Exit();
    return false;
}

bool VoxelzProfilingApp::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        cam->Update(0);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP   = cam->GetViewProjMat() * Model;

        sh->Set();

        Model = glm::mat4(1.0f);
        MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        chkmgr->Render(cam.get());

        _appContext->_window->SwapBuffers();
        return true;
    }
    return false;
}

void VoxelzProfilingApp::Exit()
{
    Application::Exit();
}

void VoxelzProfilingApp::OnWindowClose()
{

}
void VoxelzProfilingApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
}

void VoxelzProfilingApp::OnMouseMove(double x, double y)
{

}

void VoxelzProfilingApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{

}

