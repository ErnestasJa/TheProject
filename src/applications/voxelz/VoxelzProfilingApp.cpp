#include "Precomp.h"
#include "VoxelzProfilingApp.h"
#include "application/Window.h"
#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "resources/ShaderLoader.h"
#include "scenegraph/Camera.h"
#include "opengl/CubeMesh.h"
#include "opengl/GridMesh.h"
#include "Voxel/Block.h"
#include "Voxel/Chunk.h"
#include "Voxel/ChunkManager.h"
#include "gui/GUI.h"

VoxelzProfilingApp::VoxelzProfilingApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

VoxelzProfilingApp::~VoxelzProfilingApp()
{

}

static ChunkManager *chkmgr;
static AppContext *ctx;
static const int ManyChunksWidth = 2;
static const int ManyChunksHeight = 2;

#define BEGIN_BENCHMARK ctx->_timer->tick();
#define END_BENCHMARK(name) ctx->_timer->tick(); \
                      printf("Benchmarking %s is over. It took: %d ms.\n",(name),ctx->_timer->get_delta_time());

static void AddSingleChunk()
{
    BEGIN_BENCHMARK
    chkmgr->AddChunk(glm::vec3(0,0,0))->Fill();
    END_BENCHMARK("AddSingleChunk")
}

static void AddManyChunks()
{
    BEGIN_BENCHMARK
    for(int x=-ManyChunksWidth/2; x<ManyChunksWidth/2; x++)
        for(int z=-ManyChunksWidth/2; z<ManyChunksWidth/2; z++)
            for(int y=0; y<ManyChunksHeight; y++)
                chkmgr->AddChunk(glm::vec3(x,y,z))->Fill();
    END_BENCHMARK("AddManyChunks")
}

static void SingleChunkRebuild()
{
    BEGIN_BENCHMARK
    chkmgr->GetChunk(glm::vec3(0,0,0))->Rebuild();
    END_BENCHMARK("RebuildSingle")
}

static void AllChunksRebuild()
{
    BEGIN_BENCHMARK
    for(int x=-ManyChunksWidth/2; x<ManyChunksWidth/2; x++)
        for(int z=-ManyChunksWidth/2; z<ManyChunksWidth/2; z++)
            for(int y=0; y<ManyChunksHeight; y++)
                chkmgr->GetChunk(glm::vec3(x,y,z))->Rebuild();
    END_BENCHMARK("RebuildAll")
}

void VoxelzProfilingApp::Benchmark()
{
    //AddSingleChunk();

    //SingleChunkRebuild();

    //AddManyChunks();

    //AllChunksRebuild();

    u8vec4 col;

    vector<u8vec4> vec;
    vec.resize(100000000);

    BEGIN_BENCHMARK
    loop(i,100000000)
    vec[i]=(u8vec4(255,255,255,255));
    END_BENCHMARK("SHIT")

    vector<u8vec4> vec2;
    vec2.reserve(100000000);
    BEGIN_BENCHMARK
    loop(i,100000000)
    vec2.push_back(std::move(u8vec4(255,255,255,255)));
    END_BENCHMARK("SHIT")

}

bool VoxelzProfilingApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);
    ctx=_appContext;

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.5,0.5,0.7,0);

    BEGIN_BENCHMARK
    chkmgr=new ChunkManager();
    END_BENCHMARK("chkmgr gen")

    printf("\n\n--------------------------------\nPROFILE ME TIMBERS!\n--------------------------------\n");
    Benchmark();
    printf("\n--------------------------------\nTIMBERRRRR!\n--------------------------------\n");
    Exit();
    return false;
}

bool VoxelzProfilingApp::Update()
{
    return true;
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

