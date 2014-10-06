#include "precomp.h"
#include "VoxelOctreeApp.h"
#include "voxel_octree/VoxMeshManager.h"
#include "utility/SimplexNoise.h"
#include "opengl/CubeMesh.h"
#include "py/cpputils.h"
#include "py/OctreeUtils.h"
#include "py/OctreeApplicationPy.h"
#include <boost/algorithm/string/replace.hpp>


VoxelOctreeApp * VoxelOctreeApp::m_instance = nullptr;

VoxelOctreeApp * VoxelOctreeApp::Instance()
{
    return m_instance;
}

VoxelOctreeApp::VoxelOctreeApp(uint32_t argc, const char ** argv): Application(argc,argv)
{
    m_instance = this;
}

VoxelOctreeApp::~VoxelOctreeApp()
{

}

void VoxelOctreeApp::InitPython()
{
    PyImport_AppendInittab("cpputils", &PyInit_CppUtils);
    PyImport_AppendInittab("octree", &PyInit_Octree);
    PyImport_AppendInittab("oapp", &PyInit_OctreeApplication);
    Py_Initialize();

    std::string path = this->GetAbsoluteResourcePath();

    path.erase(path.length()-1);
    boost::algorithm::replace_all(path,"\\","\\\\"); //just windows things

    std::string scriptLoadString = "script_path = os.path.join(('" + path + "'),'python')";

    std::string initString = "import sys, os\n";
                initString += scriptLoadString+"\n";
                initString += "sys.path.append(script_path)\n";
                //initString += "print('Python search path: ' + str(sys.path))\n";

    PyRun_SimpleString(initString.c_str());
}

void VoxelOctreeApp::InitResources()
{
    AppContext * ctx = this->Ctx();

    sh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/solid_cube");
    cam=share(new Camera(ctx,glm::vec3(0,0,-5),glm::vec3(0,0,5),glm::vec3(0,1,0),
                         1.7777777f,45.0f,0.1,1024.0f));

    AABB aabb(glm::vec3(-0.5,-1,-0.5), glm::vec3(0.5,1,0.5));
    cube = new TCubeMesh<glm::vec3>(aabb);
    octree = new MortonOctTree<10>();
    octreeGen = new VoxMeshManager(octree);

}

bool VoxelOctreeApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);
    InitPython();

    _appContext->_window->SigKeyEvent().connect(sigc::mem_fun(this,&VoxelOctreeApp::OnKeyEvent));
    _appContext->_window->SigMouseKey().connect(sigc::mem_fun(this,&VoxelOctreeApp::OnMouseKey));
    _appContext->_window->SigMouseMoved().connect(sigc::mem_fun(this,&VoxelOctreeApp::OnMouseMove));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
    glClearColor(0.4,1,0.2,0);

    InitResources();
    AfterInit();
    _appContext->_timer->tick();
    return true;
}

void ReadBVoxFile(MortonOctTree<10> * mot, const std::string &fileName)
{
    char * buf;
    uint32_t len;
    len=helpers::read(fileName,buf);
    uint32_t * data = (uint32_t*)((void*)&buf[0]);

    uint32_t voxel_count = data[0];
    data++;

    std::cout << "File len: " << len << std::endl;
    std::cout << "Voxel count: " << voxel_count << std::endl;

    for(int i = 0; i < voxel_count; i++)
    {
        uint32_t x = data[0], y = data[1], z = data[2];
        mot->AddOrphanNode(MNode(x,y,z));
        data+=3;
    }

    delete[] buf;
}

bool VoxelOctreeApp::LoadLevel(const std::string & levelName)
{
    octree->GetChildNodes().clear();
    octreeGen->GetMeshes().clear();
    ReadBVoxFile(octree, levelName);

    octree->SortLeafNodes();
    octree->RemoveDuplicateNodes();
    std::cout << "Voxel count after duplicate removal: " << octree->GetChildNodes().size() << std::endl;
    octreeGen->GenAllChunks();
}

void VoxelOctreeApp::AfterInit()
{
    char * buf = nullptr;
    uint32_t len = helpers::read("res/voxel_octree/init.py", buf);

    if(len>0)
        PyRun_SimpleString(buf);
}

static bool renderWireframe = false;
bool VoxelOctreeApp::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        _appContext->_timer->tick();

        cam->Update(0);

        AABB aabb(glm::vec3(-0.5,-1,-0.5), glm::vec3(0.5,1,0.5));
        aabb.Translate(cam->GetPosition());

        if(octree->CheckCollisionB(aabb))
            Ctx()->_logger->log(LOG_LOG, "Camera collided with octree node");

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        sh->Set();
        glm::mat4 Model = glm::translate(glm::mat4(1.0f),cam->GetPosition()); // glm::mat4(1.0f);//
        glm::mat4 MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        cube->Render(true,false);

        Model = glm::mat4(1.0f);
        MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();

        if(renderWireframe)
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        else
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        octreeGen->RenderAllMeshes();



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

float speed = 1;
void VoxelOctreeApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
    if(action == GLFW_PRESS && key==GLFW_KEY_LEFT_SHIFT)
        speed = 0.1;

    if(action == GLFW_RELEASE && key==GLFW_KEY_LEFT_SHIFT)
        speed = 1;

    if(action == GLFW_PRESS && key==GLFW_KEY_LEFT_CONTROL)
        speed = 10;

    if(action == GLFW_RELEASE && key==GLFW_KEY_LEFT_CONTROL)
        speed = 1;

    if(key==GLFW_KEY_W)
        cam->Walk(speed);
    if(key==GLFW_KEY_S)
        cam->Walk(-speed);
    if(key==GLFW_KEY_A)
        cam->Strafe(-speed);
    if(key==GLFW_KEY_D)
        cam->Strafe(speed);

    if(key==GLFW_KEY_1)
        renderWireframe = true;
    else if(key==GLFW_KEY_2)
        renderWireframe = false;
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
            CollisionInfo info(position,lookat);
            octree->Collide(info,0,glm::ivec3(0));

            if(info.nearestDistance<INFINITY)
            {
                uint32_t x,y,z;
                decodeMK(info.node.start,x,y,z);
                this->Ctx()->_logger->log(LOG_LOG, "Collided with node at pos: [%u, %u, %u]",x,y,z);

                auto it = std::lower_bound(octree->GetChildNodes().begin(), octree->GetChildNodes().end(), info.node);
                decodeMK(it->start,x,y,z);
                this->Ctx()->_logger->log(LOG_LOG, "Collided with node at pos: [%u, %u, %u]",x,y,z);

                uint32_t mk = info.node.start & CHUNK_MASK;
                if(it!=octree->GetChildNodes().end())
                {
                    octree->GetChildNodes().erase(it);
                    octreeGen->RebuildChunk(mk);
                }
            }
            break;
        }
        case GLFW_MOUSE_BUTTON_RIGHT:
        {
            CollisionInfo info(position,lookat);
            octree->Collide(info,0,glm::ivec3(0));

            if(info.nearestDistance<INFINITY)
            {
                uint32_t x,y,z;
                decodeMK(info.node.start,x,y,z);
                this->Ctx()->_logger->log(LOG_LOG, "Collided with node at pos: [%u, %u, %u]",x,y,z);

                /// add node
                VoxelSide side = octree->GetCollisionSide(glm::vec3(x,y,z),position,lookat);
                glm::ivec3 pos = octree->VoxelSideToPosition(side);

                if((pos.x==-1&&x==0) || (pos.y==-1&&y==0) || (pos.z==-1&&z==0))
                    break;

                x+=pos.x;
                y+=pos.y;
                z+=pos.z;

                uint32_t mk = encodeMK(x,y,z);
                octree->AddNode(MNode(mk));
                octreeGen->RebuildChunk(mk&CHUNK_MASK);
            }
            break;
        }
        }
    }
}
