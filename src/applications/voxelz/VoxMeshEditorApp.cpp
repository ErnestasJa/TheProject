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
#include "opengl/CubeMesh.h"
#include "gui/GUI.h"
#include "gui/custom_elements/GUIColorPicker.h"

#include "Voxel/ChunkManager.h"

#include "Game/ParticleSystem.h"
#include "Game/ParticleEmitter.h"
#include "Game/GravityAffector.h"
#include "Game/AttractFocusAffector.h"
#include "Game/WindParticleAffector.h"

VoxMeshEditorApp::VoxMeshEditorApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

VoxMeshEditorApp::~VoxMeshEditorApp()
{

}

void VoxMeshEditorApp::InitGUI()
{
    _guiEnv=new GUIEnvironment(_appContext);
    _guiEnv->SetEventListener(this);
    _appContext->_guiEnv=this->_guiEnv;

    glm::ivec2 winSize=_appContext->_window->GetWindowSize();
    _mainWin=new GUIWindow(_guiEnv,Rect2D<int>(0,winSize.y-256,1280,256),L"The editor",true,true,true,false);

    _guiSwitches["showEditGrid"]=false;
    _guiSwitches["showVoxGrid"]=true;
    _guiSwitches["showOrigMesh"]=true;
    _guiSwitches["showVoxMesh"]=true;
    _guiSwitches["wireVoxMesh"]=false;

    GUIPane* cbxpane=new GUIPane(_guiEnv,Rect2D<int>(8,28,128,220),true);
    _labels["checkBoxPaneFile"]=new GUIStaticText(_guiEnv,Rect2D<int>(4,4,128,16),L"['s]['b]File:[b'][s']");
    _labels["checkBoxPaneFile"]->SetParent(cbxpane);
    cbxpane->SetParent(_mainWin);

    _buttons["btn_importMesh"]=new GUIButton(_guiEnv,Rect2D<int>(8,24,112,32),L"Import Mesh");
    _buttons["btn_importMesh"]->SetParent(cbxpane);

    cbxpane=new GUIPane(_guiEnv,Rect2D<int>(144,28,128,220),true);
    _labels["checkBoxPaneSwitch"]=new GUIStaticText(_guiEnv,Rect2D<int>(4,4,128,16),L"['s]['b]Switchables:[b'][s']");
    _labels["checkBoxPaneSwitch"]->SetParent(cbxpane);
    cbxpane->SetParent(_mainWin);

    uint32_t i=0;
    for(auto sw:_guiSwitches)
    {
        wchar_t wbuf[64];
        char sbuf[64];

        sprintf(sbuf,"lab_%s",sw.first.c_str());
        swprintf(wbuf,64,L"['s]%s:[s']",sw.first.c_str());
        _labels[sbuf]=new GUIStaticText(_guiEnv,Rect2D<int>(8,24+i*20,96,16),wbuf);
        _labels[sbuf]->SetParent(cbxpane);
        sprintf_s(sbuf,64,"cbx_%s",sw.first.c_str());
        _checkboxes[sbuf]=new GUICheckbox(_guiEnv,Rect2D<int>(100,24+i*20,16,16),sw.second);
        _checkboxes[sbuf]->SetParent(cbxpane);
        _checkboxes[sbuf]->SetName(sbuf);

        i++;
    }

    _colPicker=new GUIColorPicker(_guiEnv,Rect2D<int>(winSize.x-288,28,196,188),true);
    _colPicker->SetParent(_mainWin);
    _colPicker->SetName("colorPicker");

    cbxpane=new GUIPane(_guiEnv,Rect2D<int>(0,0,196,196));
    GUIStaticText* controls=new GUIStaticText(_guiEnv,Rect2D<int>(8,8,196,196),L"['s]['b]Controls:[b'][s']\n\n['s]F1 - This Menu[s']\n['s]W,A,S,D + Mouse - Move[s']\n['s]Shift - Accelerate[s']\n['s]Ctrl - Slowdown[s']\n['s]F - Focus GUI[s']\n['s]H - Toggle Editors' GUI[s']");
    controls->SetParent(cbxpane);

    cbxpane=new GUIPane(_guiEnv,Rect2D<int>(280,28,196,220),true);
    cbxpane->SetParent(_mainWin);
    GUISlider *slid=new GUISlider(_guiEnv,Rect2D<int>(8,8,160,16),1,10000,1024);
    slid->SetName("slider_EmitterSize");
    slid->SetParent(cbxpane);
    slid=new GUISlider(_guiEnv,Rect2D<int>(8,32,160,16),0.1,60,5);
    slid->SetName("slider_EmitterLife");
    slid->SetParent(cbxpane);
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
    loop(i,3)
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
    loop(i,3)
    loop(j,3)
    {
        glm::vec3 axis=glm::cross(triEdges[i],boxNormals[j]);
        glm::vec2 boxProj=Project(box.GetPoints(),axis);
        glm::vec2 triProj=Project(tri.points,axis);
        if(boxProj[1] < triProj[0] || boxProj[0] > triProj[1] )
            return false;
    }

    return true;
}

vector<CubeMesh*> boxes;

template<typename T>
void VoxelizeMesh(vector<Triangle<T> > tris,VoxelMesh* voxmesh)
{
    uint32_t trisize=tris.size();
    loop(i,tris.size())
    {
        //printf("Processing tri %u of %u...\n",i,trisize);

        AABB a;
        a.Reset(tris[i].points[0]);
        a.AddPoint(tris[i].points[1]);
        a.AddPoint(tris[i].points[2]);
        //boxes.push_back(new CubeMesh(a));
        //printf("Tri Points:\n%s\n%s\n%s\n",GLMVec3ToStr(tris[i].points[0]),GLMVec3ToStr(tris[i].points[1]),GLMVec3ToStr(tris[i].points[2]));


        //printf("AABB\nmin %s\nmax %s\ncenter %s\nhalfsize %s\n",GLMVec3ToStr(a.GetMin()),GLMVec3ToStr(a.GetMax()),GLMVec3ToStr(a.GetCenter()),GLMVec3ToStr(a.GetHalfSize()));
        glm::ivec3 amn=(glm::ivec3)glm::round((a.GetMin()));
        glm::ivec3 amx=(glm::ivec3)glm::round((a.GetMax()));
        //printf("Ranges %s %s\n",GLMVec3ToStr(amn),GLMVec3ToStr(amx));

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

        for(int32_t x=sx-1; x<=ex+1; x++)
        {
            for(int32_t y=sy-1; y<=ey+1; y++)
            {
                for(int32_t z=sz-1; z<=ez+1; z++)
                {
                    if(voxmesh->GetVoxel(x,y,z).active==false)
                    {
                        AABB voxaabb(glm::vec3(x,y,z)+glm::vec3(0.5f),glm::vec3(0.5f));
                        voxaabb.CalculatePoints();
                        if(IsIntersecting<glm::vec3>(voxaabb,tris[i]))
                            voxmesh->CreateVox(x,y,z,VecRGBToIntRGB(u8vec3(255)));
                        else
                            continue;
                    }
                    else
                        continue;
                }
            }
        }
    }
}

template<typename T>
void VoxelizeMesh(vector<Triangle<T> > tris,ChunkManager* voxmesh)
{
    uint32_t trisize=tris.size();
    loop(i,tris.size())
    {
        //printf("Processing tri %u of %u...\n",i,trisize);

        AABB a;
        a.Reset(tris[i].points[0]);
        a.AddPoint(tris[i].points[1]);
        a.AddPoint(tris[i].points[2]);
        //boxes.push_back(new CubeMesh(a));
        //printf("Tri Points:\n%s\n%s\n%s\n",GLMVec3ToStr(tris[i].points[0]),GLMVec3ToStr(tris[i].points[1]),GLMVec3ToStr(tris[i].points[2]));


        //printf("AABB\nmin %s\nmax %s\ncenter %s\nhalfsize %s\n",GLMVec3ToStr(a.GetMin()),GLMVec3ToStr(a.GetMax()),GLMVec3ToStr(a.GetCenter()),GLMVec3ToStr(a.GetHalfSize()));
        glm::ivec3 amn=(glm::ivec3)glm::round((a.GetMin()));
        glm::ivec3 amx=(glm::ivec3)glm::ceil((a.GetMax()));
        //printf("Ranges %s %s\n",GLMVec3ToStr(amn),GLMVec3ToStr(amx));

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

        for(int32_t x=sx-1; x<=ex+1; x++)
        {
            for(int32_t y=sy-1; y<=ey+1; y++)
            {
                for(int32_t z=sz-1; z<=ez+1; z++)
                {
                    if(voxmesh->GetBlock(glm::ivec3(x,y,z))==Chunk::EMPTY_BLOCK)
                    {
                        //printf("Definitely empty\n");
                        AABB voxaabb(glm::vec3(x,y,z)+glm::vec3(0.5f),glm::vec3(0.5f));
                        voxaabb.CalculatePoints();
                        if(IsIntersecting<glm::vec3>(voxaabb,tris[i]))
                            voxmesh->SetBlock(glm::ivec3(x,y,z),EBT_STONE,true);
                        else
                            continue;
                    }
                    else
                        continue;
                }
            }
        }
    }
}

ChunkManager* cmg;
bool VoxMeshEditorApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    _appContext->_input=new InputHandler(_appContext->_window);

    _appContext->_window->SigKeyEvent().connect(sigc::mem_fun(this,&VoxMeshEditorApp::OnKeyEvent));
    _appContext->_window->SigMouseKey().connect(sigc::mem_fun(this,&VoxMeshEditorApp::OnMouseKey));
    _appContext->_window->SigMouseMoved().connect(sigc::mem_fun(this,&VoxMeshEditorApp::OnMouseMove));

    shader_loader* shaderLoader=new shader_loader(_appContext->_logger);

    _defaultShader=shaderLoader->load("res/engine/shaders/solid_unlit");
    _meshShader=shaderLoader->load("res/engine/shaders/solid_unlit_tex");
    _voxShader=shaderLoader->load("res/engine/shaders/voxel");
    _particleShader=shaderLoader->load("res/engine/shaders/voxelparticle");

    delete shaderLoader;

    _grid=new VoxMeshGrid(glm::vec3(128),8);
    _axisGrid=new GridMesh(1,128);

    _cam=new Camera(_appContext,glm::vec3(0,32,150),glm::vec3(0),glm::vec3(0,1,0));

    mesh_loader* meshLoader=new mesh_loader(_appContext->_logger);
    meshLoader->add_loader(new iqmloader(_appContext->_logger));
    _iqmMesh=meshLoader->load("res/mill.iqm");
    _iqmMesh->RecalculateAABB<glm::vec3>();

    uint32_t gridSize=1;
    if(_iqmMesh->aabb.GetCenter()!=glm::vec3(0))
    {
        _iqmMesh->HardMove<glm::vec3>(glm::vec3(0)-_iqmMesh->aabb.GetCenter());
    }
    glm::vec3 hs=_iqmMesh->aabb.GetHalfSize()*2.f;
    float scale=(float)(gridSize)/glm::abs(glm::max(glm::max(hs.x,hs.y),hs.z));
    _iqmMesh->HardScale<glm::vec3>(glm::vec3(scale));
    //_iqmMesh->HardMove<glm::vec3>(_iqmMesh->aabb.GetHalfSize());

    AABB bb=_iqmMesh->aabb;
    boxes.push_back(new CubeMesh(_iqmMesh->aabb));

    //_voxMesh=new VoxelMesh(u16vec3(gridSize));

    vector<Triangle<glm::vec3> > vec=_iqmMesh->GetTriangles<glm::vec3,uint32_t>();
    printf("Total triangles: %u\n",vec.size());

//    VoxelizeMesh(vec,_voxMesh);
    cmg=new ChunkManager();
    VoxelizeMesh(vec,cmg);
    cmg->FlagGenerated();

    _particleSystem=new ParticleSystem();
    _emitter=new ParticleEmitter(glm::vec3(-128,25,0),glm::vec3(1,0,0),15,5,100,1024,true);
    _emitter->AddParticleAffector(new GravityAffector());
    _emitter->AddParticleAffector(new WindParticleAffector(AABB(glm::vec3(0,0,0),glm::vec3(1024,128,1024)),glm::vec3(-1,0,0),15));
    _emitter->AddParticleAffector(new AttractFocusAffector(glm::vec3(0,100,0),50));
    _particleSystem->AddEmitter(_emitter);

    ParticleEmitter* em=new ParticleEmitter(glm::vec3(10,0,0),glm::vec3(0,1,0),15,50,1,4096);
    em->AddParticleAffector(new GravityAffector());
    _particleSystem->AddEmitter(em);

    //_voxMesh->UpdateMesh();

    InitGUI();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.5,0.5,0.7,0);

    //AABB bb(glm::vec3(0.5),glm::vec3(0.5));
    //boxes.push_back(new CubeMesh(bb));
    //printf("AABB\nmin %s\nmax %s\ncenter %s\nhalfsize %s\n",GLMVec3ToStr(bb.GetMin()),GLMVec3ToStr(bb.GetMax()),GLMVec3ToStr(bb.GetCenter()),GLMVec3ToStr(bb.GetHalfSize()));

    return true;
}

bool VoxMeshEditorApp::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        _appContext->_timer->tick();
        float dt=(float)_appContext->_timer->get_delta_time()/1000.f;
        _cam->Update(dt);
        _particleSystem->Update(dt);
        HandleMovement(dt);

        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

        _defaultShader->Set();
        glm::mat4 Model(1.0f);
        glm::mat4 MVP   = _cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        if(_guiSwitches["showVoxGrid"])
            _axisGrid->render_lines();
        if(_guiSwitches["showEditGrid"])
        {
            glEnable(GL_BLEND);
            _grid->render_lines();
            glDisable(GL_BLEND);
        }

        MVar<glm::mat4>(0, "mvp", MVP).Set();
        for(auto b:boxes)
        {
            b->Render(true);
        }

        if(_guiSwitches["showOrigMesh"])
        {
            MVar<glm::mat4>(0, "mvp", MVP).Set();
            _iqmMesh->Render();
        }

        if(_guiSwitches["showVoxMesh"])
        {
            _voxShader->Set();
            MVar<glm::mat4>(0, "mvp", MVP).Set();
            cmg->Render(_cam,_voxShader,_guiSwitches["wireVoxMesh"]);
        }
        _particleShader->Set();
        if(_particleShader->getparam("M")!=-1) MVar<glm::mat4>(_particleShader->getparam("M"), "M", glm::mat4(1.f)).Set();
        if(_particleShader->getparam("V")!=-1) MVar<glm::mat4>(_particleShader->getparam("V"), "V", _cam->GetViewMat()).Set();
        if(_particleShader->getparam("P")!=-1) MVar<glm::mat4>(_particleShader->getparam("P"), "P", _cam->GetProjectionMat()).Set();
        _particleSystem->Render();

        _guiEnv->Render();

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
    if(_cam->IsFPS())
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
}

void VoxMeshEditorApp::OnWindowClose()
{

}
void VoxMeshEditorApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
    if(key==GLFW_KEY_F&&action==GLFW_RELEASE)
    {
        _cam->SetFPS(!_cam->IsFPS());
    }
    if(key==GLFW_KEY_H&&action==GLFW_RELEASE)
    {
        _mainWin->SetVisible(!_mainWin->IsVisible());
    }
}

bool draggingMouse;

void VoxMeshEditorApp::OnMouseMove(double x, double y)
{

}

void VoxMeshEditorApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{
    switch(button)
    {
    case GLFW_MOUSE_BUTTON_1:
        switch(action)
        {
        case GLFW_PRESS:
            draggingMouse=true;
            break;
        case GLFW_RELEASE:
            draggingMouse=false;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

bool VoxMeshEditorApp::OnEvent(const GUIEvent& e)
{
    switch(e.GetType())
    {
    case gui_event_type::checkbox_state_changed:
        if(e.get_caller()->GetName().compare("cbx_showEditGrid")==0)
            _guiSwitches["showEditGrid"]=_checkboxes["cbx_showEditGrid"]->IsChecked();

        if(e.get_caller()->GetName().compare("cbx_showVoxGrid")==0)
            _guiSwitches["showVoxGrid"]=_checkboxes["cbx_showVoxGrid"]->IsChecked();

        if(e.get_caller()->GetName().compare("cbx_showOrigMesh")==0)
            _guiSwitches["showOrigMesh"]=_checkboxes["cbx_showOrigMesh"]->IsChecked();

        if(e.get_caller()->GetName().compare("cbx_showVoxMesh")==0)
            _guiSwitches["showVoxMesh"]=_checkboxes["cbx_showVoxMesh"]->IsChecked();

        if(e.get_caller()->GetName().compare("cbx_wireVoxMesh")==0)
            _guiSwitches["wireVoxMesh"]=_checkboxes["cbx_wireVoxMesh"]->IsChecked();
        break;

    case gui_event_type::scrollbar_changed:
        if(e.get_caller()->GetName().compare("slider_EmitterSize")==0)
        {
            uint32_t newValue=(uint32_t)glm::round(((GUISlider*)e.get_caller())->get_value());
            _emitter->SetSize(newValue);
            //_appContext->_logger->log(LOG_DEBUG,"%u",newValue);
        }

        if(e.get_caller()->GetName().compare("slider_EmitterLife")==0)
        {
            uint32_t newValue=(uint32_t)glm::round(((GUISlider*)e.get_caller())->get_value());
            _emitter->SetParticleLife(newValue);
            //_appContext->_logger->log(LOG_DEBUG,"%u",newValue);
        }
        break;
    default:
        break;

    }
}

