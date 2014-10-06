#include "precomp.h"
#include "VoxelOctreeBenchmarkApp.h"
#include "voxel_octree/VoxMeshManager.h"
#include "utility/SimplexNoise.h"


VoxelOctreeBenchmarkApp::VoxelOctreeBenchmarkApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

VoxelOctreeBenchmarkApp::~VoxelOctreeBenchmarkApp()
{

}

void VoxelOctreeBenchmarkApp::InitOctree()
{
    AppContext * ctx = this->Ctx();
    sh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/solid_color");
    cam=share(new Camera(ctx,glm::vec3(0,0,-5),glm::vec3(0,0,5),glm::vec3(0,1,0)));

    mesh = MeshPtr(new Mesh());
    IndexBufferObject<uint32_t> * ibo = new IndexBufferObject<uint32_t>();
    BufferObject<glm::vec3> *vbo = new BufferObject<glm::vec3>();
    BufferObject<glm::vec3> *cbo = new BufferObject<glm::vec3>();
    mesh->buffers[Mesh::POSITION] = vbo;
    mesh->buffers[Mesh::INDICES] = ibo;
    mesh->buffers[Mesh::COLOR] = cbo;
    mesh->Init();

    octree = new MortonOctTree<10>();
    octreeGen = new VoxMeshManager(octree);
}

#define START(x) timer->tick(); x = timer->get_real_time()
#define END(x) timer->tick(); x = timer->get_real_time() - x
#define DEBUG_PRINT(x) Ctx()->_logger->log(LOG_LOG, "%s took %ums time", #x, x)
void VoxelOctreeBenchmarkApp::BuildOctree()
{
    uint32_t time, addTime, sortTime, rebuildTime, generationTime;

    START(addTime);
        octree->GetChildNodes().reserve(256*256*256);
        loop(i,256)
            loop(j,256)
                loop(k,256)
                    octree->AddOrphanNode(MNode(k,j,i));
    END(addTime);

    START(sortTime);
        octree->SortLeafNodes();
    END(sortTime);

    START(generationTime);
        octreeGen->GenAllChunks();
    END(generationTime);

    DEBUG_PRINT(addTime);
    DEBUG_PRINT(sortTime);
    DEBUG_PRINT(generationTime);
}

#undef START
#undef END
#undef DEBUG_PRINT

void VoxelOctreeBenchmarkApp::FreeOctree()
{
    delete octree;
    delete octreeGen;
    mesh = MeshPtr();
}



bool VoxelOctreeBenchmarkApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);
    timer = Ctx()->_timer;

    InitOctree();
    BuildOctree();
    FreeOctree();

    return true;
}

bool VoxelOctreeBenchmarkApp::Update()
{
    return false;
}

void VoxelOctreeBenchmarkApp::Exit()
{
    Application::Exit();
}

void VoxelOctreeBenchmarkApp::OnWindowClose()
{

}
void VoxelOctreeBenchmarkApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
}

void VoxelOctreeBenchmarkApp::OnMouseMove(double x, double y)
{
}

void VoxelOctreeBenchmarkApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{
}
