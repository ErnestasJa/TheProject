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
#include "boost/pool/object_pool.hpp"
#include "Voxel/VoxelSprite.h"
#include "resources/ImageLoader.h"

VoxelzProfilingApp::VoxelzProfilingApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

VoxelzProfilingApp::~VoxelzProfilingApp()
{

}

static ChunkManager *chkmgr;
static AppContext *ctx;
static const int ManyChunksWidth = 16;
static const int ManyChunksHeight = 16;

#define BEGIN_BENCHMARK ctx->_timer->tick();
#define END_BENCHMARK(name) ctx->_timer->tick(); \
                      printf("Benchmarking %s is over. It took: %d ms.\n",(name),ctx->_timer->get_delta_time());
#define BENCHMARK_INTERRUPT std::cout << "Enter c to continue" << std::endl; while(std::cin.get()!='c') {}

static void AddSingleChunk()
{
    BEGIN_BENCHMARK
    END_BENCHMARK("AddSingleChunk")
}

static void AddManyChunks()
{
    BEGIN_BENCHMARK
    END_BENCHMARK("AddManyChunks")
}

static void SingleChunkRebuild()
{
    BEGIN_BENCHMARK
    END_BENCHMARK("RebuildSingle")
}

static void AllChunksRebuild()
{
    BEGIN_BENCHMARK
    END_BENCHMARK("RebuildAll")
}

static void RemoveManyChunks()
{
    BEGIN_BENCHMARK
    END_BENCHMARK("AddManyChunks")
}

void VoxelzProfilingApp::Benchmark()
{
    BEGIN_BENCHMARK
    ChunkManager* cmgr=new ChunkManager();
    loopi(10)
    {
        cmgr->AddSuperChunk(glm::ivec3(i));
    }

    BENCHMARK_INTERRUPT

    loopi(10)
    {
        cmgr->RemoveSuperChunk(glm::ivec3(i));
    }

    BENCHMARK_INTERRUPT
    //cmgr->GetSuperChunk(glm::ivec3(0))->SaveToFile();
    delete cmgr;
    END_BENCHMARK("fullhdimagegen")
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
    //chkmgr=new ChunkManager();
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

