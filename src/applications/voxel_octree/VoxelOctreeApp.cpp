#include "precomp.h"
#include "VoxelOctreeApp.h"
#include "voxel_octree/VoxMeshManager.h"
#include "utility/SimplexNoise.h"


VoxelOctreeApp::VoxelOctreeApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

VoxelOctreeApp::~VoxelOctreeApp()
{

}

void VoxelOctreeApp::InitPlaneMesh()
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

    for(uint32_t i = 0; i < 1024; i++)
        for(uint32_t j = 0; j < 4; j++)
            for(uint32_t k = 0; k < 1024; k++)
                octree->AddOrphanNode(MNode(k,j,i));

    uint32_t size = octree->GetChildNodes().size();

    /*
    for(auto it = octree->GetChildNodes().begin(); it != octree->GetChildNodes().end(); it++ )
    {
        MNode node = (*it);
        if(node.start!=0)
            std::cout << "Start = " << node.start << std::endl;
    }*/

    octree->SortLeafNodes();
    octree->RebuildTree();
    octreeGen->GenMesh(mesh);
}

bool VoxelOctreeApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    _appContext->_window->SigKeyEvent().connect(sigc::mem_fun(this,&VoxelOctreeApp::OnKeyEvent));
    _appContext->_window->SigMouseKey().connect(sigc::mem_fun(this,&VoxelOctreeApp::OnMouseKey));
    _appContext->_window->SigMouseMoved().connect(sigc::mem_fun(this,&VoxelOctreeApp::OnMouseMove));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    //glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    glClearColor(0.4,1,0.2,0);

    InitPlaneMesh();

    _appContext->_timer->tick();
    return true;
}

bool VoxelOctreeApp::Update()
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

        _appContext->_window->SwapBuffers();
        return true;
    }
    return false;
}

void VoxelOctreeApp::Exit()
{
    Application::Exit();
}

void VoxelOctreeApp::OnWindowClose()
{

}
void VoxelOctreeApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
    if(key==GLFW_KEY_W)
        cam->Walk(1);
    if(key==GLFW_KEY_S)
        cam->Walk(-1);
    if(key==GLFW_KEY_A)
        cam->Strafe(-1);
    if(key==GLFW_KEY_D)
        cam->Strafe(1);
}

void VoxelOctreeApp::OnMouseMove(double x, double y)
{

}

void VoxelOctreeApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{
    glm::vec3 position=cam->GetPosition();
    glm::vec3 lookat=glm::normalize(cam->GetLook());

    if(action==GLFW_PRESS)
    {
        switch(button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
        {
            MNode n;
            if(octree->Collide(n,position,lookat))
            {
                uint32_t x,y,z;
                decodeMK(n.start,x,y,z);
                this->Ctx()->_logger->log(LOG_LOG, "Collided with node at pos: [%u, %u, %u]",x,y,z);

                auto it = std::lower_bound(octree->GetChildNodes().begin(), octree->GetChildNodes().end(), n);
                if(it!=octree->GetChildNodes().end())
                {
                    octree->GetChildNodes().erase(it);
                    ///for now the erase means full rebuild
                    octree->RebuildTree();
                    octreeGen->GenMesh(mesh);
                }
            }
            break;
        }
        case GLFW_MOUSE_BUTTON_RIGHT:
        {
            MNode n;
            if(octree->Collide(n,position,lookat))
            {
                uint32_t x,y,z;
                decodeMK(n.start,x,y,z);
                this->Ctx()->_logger->log(LOG_LOG, "Collided with node at pos: [%u, %u, %u]",x,y,z);

                /// add node
                VoxelSide side = octree->GetCollisionSide(glm::vec3(x,y,z),position,lookat);
                glm::ivec3 pos = octree->VoxelSideToPosition(side);

                if((pos.x==-1&&x==0) || (pos.y==-1&&y==0) || (pos.z==-1&&z==0))
                    break;

                x+=pos.x;
                y+=pos.y;
                z+=pos.z;

                octree->AddNode(MNode(x,y,z));
                octreeGen->GenMesh(mesh);
            }
            break;
        }
        }
    }
}
