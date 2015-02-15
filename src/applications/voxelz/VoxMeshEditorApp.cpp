#include "Precomp.h"
#include "VoxMeshEditorApp.h"
#include "Voxel/VoxelMesh.h"
#include "VoxMeshGrid.h"
#include "application/Window.h"
#include "application/InputHandler.h"
#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "resources/ShaderLoader.h"
#include "resources/MeshLoader.h"
#include "resources/IQMLoader.h"
#include "scenegraph/Camera.h"
#include "resources/ImageLoader.h"
#include "opengl/GridMesh.h"
#include "gui/GUI.h"
#include "gui/custom_elements/GUIColorPicker.h"
#include "physics/Physics.h"

VoxMeshEditorApp::VoxMeshEditorApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

VoxMeshEditorApp::~VoxMeshEditorApp()
{

}

void VoxMeshEditorApp::InitGUI()
{
    _guiEnv=new GUIEnvironment(_appContext);
    _appContext->_guiEnv=this->_guiEnv;

    glm::ivec2 winSize=_appContext->_window->GetWindowSize();
    _mainWin=new GUIWindow(_guiEnv,Rect2D<int>(winSize.x-512,0,512,512),L"The editor",true,false,true,false);

    _guiSwitches["showGrid"]=true;
    _guiSwitches["showMesh"]=true;
    _guiSwitches["showVoxMesh"]=true;
    _guiSwitches["wireVoxMesh"]=false;
}

glm::vec2 Project(vector<glm::vec3> points,const glm::vec3 &axis)
{
    float min=999999.f;
    float max=-999999.f;

    for(uint32_t i=0; i<points.size(); i++)
    {
        float val=glm::dot(points[i],axis);
        if(val < min) min=val;
        if(val > max) max=val;
    }

    return glm::vec2(min,max);
}

template <typename T>
bool IsIntersecting(AABB box,Triangle<T> tri)
{
    //test aabb normals
    glm::vec3 boxNormals[]= {glm::vec3(1.f,0.f,0.f),glm::vec3(0.f,1.f,0.f),glm::vec3(0.f,0.f,1.f)};
    loopi(i,3)
    {
        glm::vec3 n=boxNormals[i];
        glm::vec2 proj=Project(tri.points,n);
        if(proj[1] < box.GetMin()[i] || proj[0] > box.GetMax()[i])
            return false; // no intersection possible
    }

    //test triangle normal
    float triOffset=glm::dot(tri.normal,tri.points[0]);
    glm::vec2 proj=Project(box.GetPoints(),tri.normal);
    if(proj[1] < triOffset || proj[0] > triOffset)
        return false; //no intersection possible

    // test nine edge cross products
    T triEdges[]= {tri.points[0]-tri.points[1],tri.points[1]-tri.points[2],tri.points[2]-tri.points[0]};
    loopi(i,3)
    loopi(j,3)
    {
        glm::vec3 axis=glm::cross(triEdges[i],boxNormals[j]);
        glm::vec2 boxProj=Project(box.GetPoints(),axis);
        glm::vec2 triProj=Project(tri.points,axis);
        if(boxProj[1] <= triProj[0] || boxProj[0] >= triProj[1] )
            return false;
    }

    return true;
}

template<typename T>
void VoxelizeMesh(vector<Triangle<T> > tris,VoxelMesh* voxmesh)
{
    uint32_t trisize=tris.size();
    loopi(i,tris.size())
    {
        printf("Processing tri %u of %u...\n",i,trisize);

        AABB a;
        a.AddPoint(tris[i].points[0]);
        a.AddPoint(tris[i].points[1]);
        a.AddPoint(tris[i].points[2]);
        //a.CalculatePoints();

        glm::ivec3 amn=(glm::ivec3)glm::floor(a.GetCenter()-a.GetHalfSize());
        glm::ivec3 amx=(glm::ivec3)glm::ceil(a.GetCenter()+a.GetHalfSize());

        int32_t sx,ex,sy,ey,sz,ez;

        sx=amn.x;
        sy=amn.y;
        sz=amn.z;

        ex=amx.x;
        ey=amx.y;
        ez=amx.z;

        if(ex<sx)
        {
            int32_t tmp=sx;
            sx=ex;
            ex=tmp;
        }

        if(ey<sy)
        {
            int32_t tmp=sy;
            sy=ey;
            ey=tmp;
        }

        if(ez<sz)
        {
            int32_t tmp=sz;
            sz=ez;
            ez=tmp;
        }

        for(int32_t x=sx; x<=ex; x++)
        {
            for(int32_t y=sy; y<=ey; y++)
            {
                for(int32_t z=sz; z<=ez; z++)
                {
                    if(voxmesh->GetVoxel(x,y,z).active==false)
                    {
                        AABB voxaabb(glm::vec3(x,y,z)+glm::vec3(0.5f),glm::vec3(0.5f));
                        voxaabb.CalculatePoints();
                        if(IsIntersecting<glm::vec3>(voxaabb,tris[i]))
                            voxmesh->CreateVox(x,y,z,VecRGBToIntRGB(u8vec3(255)));
                        else
                            continue;//voxmesh->CreateVox(x,y,z,VecRGBToIntRGB(u8vec3(255,0,0)));
                    }
                    else
                        continue;
                }
            }
        }
    }
}


bool VoxMeshEditorApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    _appContext->_input=new InputHandler(_appContext->_window);

    _defaultShader=(new shader_loader(_appContext->_logger))->load("res/engine/shaders/solid_unlit");
    _meshShader=(new shader_loader(_appContext->_logger))->load("res/engine/shaders/solid_unlit_tex");
    _voxShader=(new shader_loader(_appContext->_logger))->load("res/engine/shaders/voxel");

    _grid=new VoxMeshGrid(glm::vec3(64),8);
    _axisGrid=new GridMesh(1);

    _cam=new Camera(_appContext,glm::vec3(0,32,150),glm::vec3(0),glm::vec3(0,1,0));

    mesh_loader* meshLoader=new mesh_loader(_appContext->_logger);
    meshLoader->add_loader(new iqmloader(_appContext->_logger));
    _iqmMesh=meshLoader->load("res/mill.iqm");

    _iqmMesh->HardScale<glm::vec3>(glm::vec3(3.f));
    //_iqmMesh->HardMove<glm::vec3>(_iqmMesh->aabb.GetHalfSize());

    _voxMesh=new VoxelMesh(u16vec3(128));


    vector<Triangle<glm::vec3> > vec=_iqmMesh->GetTriangles<glm::vec3,uint32_t>();
    printf("Total triangles: %u\n",vec.size());

    VoxelizeMesh(vec,_voxMesh);

    _voxMesh->UpdateMesh();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.5,0.5,0.7,0);

    return true;
}

bool VoxMeshEditorApp::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        _appContext->_timer->tick();
        float dt=(float)_appContext->_timer->get_delta_time()/1000.f;
        _cam->Update(dt);
        HandleMovement(dt);

        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

        _defaultShader->Set();
        glm::mat4 Model(1.0f);
        glm::mat4 MVP   = _cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        _axisGrid->render_lines();
        //_grid->render_lines();

        _meshShader->Set();
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        _iqmMesh->Render();

        _voxShader->Set();
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        _voxMesh->Render();

        _appContext->_window->SwapBuffers();
        return true;
    }
    return false;
}

void VoxMeshEditorApp::Exit()
{
    Application::Exit();
}

void VoxMeshEditorApp::HandleMovement(float dt)
{
    float speed=16.f;
    InputHandler* han=_appContext->_input;
    if(han->IsKeyDown(GLFW_KEY_LEFT_SHIFT))
        speed=32.f;
    else
        speed=16.f;
    if(han->IsKeyDown(GLFW_KEY_W))
        _cam->Walk(speed*dt);
    if(han->IsKeyDown(GLFW_KEY_S))
        _cam->Walk(-speed*dt);
    if(han->IsKeyDown(GLFW_KEY_A))
        _cam->Strafe(-speed*dt);
    if(han->IsKeyDown(GLFW_KEY_D))
        _cam->Strafe(speed*dt);
}

void VoxMeshEditorApp::OnWindowClose()
{

}
void VoxMeshEditorApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
}

void VoxMeshEditorApp::OnMouseMove(double x, double y)
{

}

void VoxMeshEditorApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{

}

