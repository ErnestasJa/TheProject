#include "precomp.h"
#include "VoxelzApp.h"
#include "application/window.h"
#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "OpenGl/FrameBufferObject.h"
#include "OpenGl/RenderBufferObject.h"
#include "resources/ShaderLoader.h"
#include "scenegraph/Camera.h"
#include "Opengl/CubeMesh.h"
#include "Opengl/GridMesh.h"
#include "Voxel/Block.h"
#include "Voxel/Chunk.h"
#include "Voxel/ChunkManager.h"
#include "GUI/GUI.h"

VoxelzApp::VoxelzApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

VoxelzApp::~VoxelzApp()
{

}

static MeshPtr mesh;
static ShaderPtr sh,vsh,qsh,gbsh;
static CameraPtr cam;
static CubeMesh *cub,*smallcub;
static GridMesh *grid;
static ChunkManager *chkmgr;
static GUIEnvironment *env;
static gui_image *guiImg;
static FrameBufferObject* GBuffer;
static TexturePtr GBdepth,GBdiffuse,GBnormal,GBposition,GBtexcoord;
static glm::vec3 voxpos,newvoxpos,pointpos;
static bool validvoxel,wireframe;
static int face;

bool InitPostProc()
{
//    RenderBufferObject* rbo=new RenderBufferObject();
//    rbo->Init(GL_DEPTH_COMPONENT16,1280,720);
    /// DEPTH
    GBdepth=share(new Texture());
    GL_CHECK(GBdepth->Init(nullptr,GL_TEXTURE_2D,GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT,1280,768));
    /// DIFFUSE
    GBdiffuse=share(new Texture());
    GL_CHECK(GBdiffuse->Init(nullptr,GL_TEXTURE_2D,GL_RGB,GL_RGB,1280,768));
    /// NORMALS
    GBnormal=share(new Texture());
    GL_CHECK(GBnormal->Init(nullptr,GL_TEXTURE_2D,GL_RGB,GL_RGB,1280,768));
    /// POSITION
    GBposition=share(new Texture());
    GL_CHECK(GBposition->Init(nullptr,GL_TEXTURE_2D,GL_RGB,GL_RGB,1280,768));
    /// TEXCOORD
    GBtexcoord=share(new Texture());
    GL_CHECK(GBtexcoord->Init(nullptr,GL_TEXTURE_2D,GL_RGB,GL_RGB,1280,768));

    guiImg=new gui_image(env,Rect2D<int>(1280-320,0,320,192),GBdepth,false);
    guiImg=new gui_image(env,Rect2D<int>(1280-640,0,320,192),GBdiffuse);
    guiImg=new gui_image(env,Rect2D<int>(1280-320,192,320,192),GBnormal);
    guiImg=new gui_image(env,Rect2D<int>(1280-320,384,320,192),GBposition);
    guiImg=new gui_image(env,Rect2D<int>(1280-320,576,320,192),GBtexcoord);

    GBuffer=new FrameBufferObject();
    GBuffer->Init();
    GBuffer->Set();

    GBuffer->AttachDepthTexture(GBdepth);
    GBuffer->AttachTexture(0,GBdiffuse);
    GBuffer->AttachTexture(1,GBnormal);
    GBuffer->AttachTexture(2,GBposition);
    GBuffer->AttachTexture(3,GBtexcoord);

    GBuffer->EnableBuffer(0);
    GBuffer->EnableBuffer(1);
    GBuffer->EnableBuffer(2);
    GBuffer->EnableBuffer(3);

    GBuffer->Unset();
    if(!GBuffer->IsComplete()) return false;

    return true;
}

void InitPlaneMesh(AppContext * ctx)
{
    validvoxel=false;
    wireframe=false;
    voxpos=glm::vec3(0);

    BufferObject<glm::vec3> * pos = new BufferObject<glm::vec3>();
    pos->data.push_back(glm::vec3(-1, 1, 0));
    pos->data.push_back(glm::vec3( 1, 1, 0));
    pos->data.push_back(glm::vec3( -1,-1, 0));
    pos->data.push_back(glm::vec3(1,-1, 0));

    BufferObject<glm::vec2> *uv = new BufferObject<glm::vec2>();
    uv->data.push_back(glm::vec2(0,1));
    uv->data.push_back(glm::vec2(1,1));
    uv->data.push_back(glm::vec2(0,0));
    uv->data.push_back(glm::vec2(1,0));

    IndexBufferObject<uint32_t> * id = new IndexBufferObject<uint32_t>();
    id->data.push_back(0);
    id->data.push_back(2);
    id->data.push_back(1);
    id->data.push_back(2);
    id->data.push_back(3);
    id->data.push_back(1);

    mesh = share(new Mesh());
    mesh->buffers[Mesh::POSITION] = pos;
    mesh->buffers[Mesh::TEXCOORD] = uv;
    mesh->buffers[Mesh::INDICES] = id;
    mesh->Init();

    sh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/solid_unlit");
    vsh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/voxelphong");
    qsh = (new shader_loader(ctx->_logger))->load("res/quad");
    gbsh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/gbuffer");

    cam=share(new Camera(ctx,glm::vec3(0,128,0),glm::vec3(0,128,32),glm::vec3(0,1,0)));

    env=new GUIEnvironment(ctx->_window,ctx->_logger);
    gui_pane* pan=new gui_pane(env,Rect2D<int>(0,0,200,200),true);

    gui_static_text* texts[10];

    std::stringstream ss;

    loopi(10)
    {
        ss<<i;
        texts[i]=new gui_static_text(env,Rect2D<int>(0,i*20,200,20),L"",glm::vec4(1),false,true);
        texts[i]->SetName(ss.str());
        texts[i]->SetParent(pan);
        ss.str(std::string()); ///clear stream
    }

    cub=new CubeMesh(1);
    smallcub=new CubeMesh(0.25);

    grid=new GridMesh(1.f,1024,16);
    Timer timer;

    chkmgr=new ChunkManager();
}

bool VoxelzApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    _appContext->_window->SigKeyEvent().connect(sigc::mem_fun(this,&VoxelzApp::OnKeyEvent));
    _appContext->_window->SigMouseKey().connect(sigc::mem_fun(this,&VoxelzApp::OnMouseKey));
    _appContext->_window->SigMouseMoved().connect(sigc::mem_fun(this,&VoxelzApp::OnMouseMove));

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.5,0.5,0.7,0);

    _appContext->_timer->tick();

    InitPlaneMesh(_appContext);
    if(!InitPostProc())
        return false;

    _appContext->_timer->tick();

    printf("Building took: %d ms\n",_appContext->_timer->get_delta_time());
    return true;
}

bool VoxelzApp::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        _appContext->_timer->tick();

        cam->Update(0);
        if(wireframe==true)
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        }
        GBuffer->Set();
        GBuffer->EnableBuffer(0);
        GBuffer->EnableBuffer(1);
        glClearColor(0.45, 0.45, 0.45, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 MVP   = cam->GetViewProjMat() * Model;

        sh->Set();

        Model = glm::mat4(1.0f);
        Model = glm::translate(voxpos);
        MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        cub->Render(true);

        Model = glm::mat4(1.0f);
        Model = glm::translate(newvoxpos);
        MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        cub->Render(true);

        Model = glm::mat4(1.0f);
        Model = glm::translate(pointpos);
        MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        smallcub->Render(true);

        Model = glm::mat4(1.0f);
        MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        grid->render_lines();

        gbsh->Set();
        Model = glm::mat4(1.0f);
        MVP   = cam->GetViewProjMat() * Model;
//        if(vsh->getparam("v_inv")!=-1)
//            MVar<glm::mat4>(vsh->getparam("v_inv"), "v_inv", glm::inverse(cam->GetViewMat())).Set();
        MVar<glm::mat4>(vsh->getparam("P"), "P", cam->GetProjectionMat()).Set();
        MVar<glm::mat4>(vsh->getparam("V"), "V", cam->GetViewMat()).Set();
        chkmgr->Render(cam.get(),vsh);

        if(wireframe==true)
        {
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
        }
        GBuffer->Unset();

        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        qsh->Set();

        GBdiffuse->Set(0);
        mesh->Render();

        env->Render();

        _appContext->_window->SwapBuffers();
        return true;
    }
    return false;
}

void VoxelzApp::Exit()
{
    Application::Exit();
}

void VoxelzApp::OnWindowClose()
{

}
void VoxelzApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
    if(key==GLFW_KEY_W)
        cam->Walk(1);
    if(key==GLFW_KEY_S)
        cam->Walk(-1);
    if(key==GLFW_KEY_A)
        cam->Strafe(-1);
    if(key==GLFW_KEY_D)
        cam->Strafe(1);
    if(key==GLFW_KEY_SPACE&&action==GLFW_RELEASE)
    {
        chkmgr->Explode(voxpos,5);
    }
    if(key==GLFW_KEY_Z&&action==GLFW_RELEASE)
    {
        wireframe=!wireframe;
    }
}

void VoxelzApp::OnMouseMove(double x, double y)
{
    /* Very naive ray casting algorithm to find out which block we are looking at */

    glm::vec3 position=cam->GetPosition();
    glm::vec3 lookat=glm::normalize(cam->GetLook());

    glm::vec3 testpos = position;
    glm::vec3 prevpos = position;
    int mx,my,mz;
    wchar_t buf[256];
    /// 50/10 ~ 5 voxels
    for(int i = 0; i < 100; i++)
    {
        /* Advance from our currect position to the direction we are looking at, in small steps */
        prevpos = testpos;
        testpos += lookat * 0.1f;

        mx = glm::floor(testpos.x);
        my = glm::floor(testpos.y);
        mz = glm::floor(testpos.z);

        /* If we find a block that is not air, we are done */
        if(chkmgr->GetBlock(glm::vec3(mx, my, mz)).GetBlockType()!=EBT_DEFAULT)
        {
            validvoxel=true;
            break;
        }
    }

    swprintf(buf,L"LookAt %.2f %.2f %.2f",lookat.x,lookat.y,lookat.z);
    env->get_element_by_name_t<gui_static_text>("0")->set_text(buf);

    glm::vec3 aa=WorldToChunkCoords(glm::vec3(mx,my,mz)),bb=ChunkSpaceCoords(glm::vec3(mx,my,mz));

    swprintf(buf,L"Chunk %.2f %.2f %.2f",aa.x,aa.y,aa.z);
    env->get_element_by_name_t<gui_static_text>("1")->set_text(buf);

    swprintf(buf,L"Chunk Coords %.2f %.2f %.2f",bb.x,bb.y,bb.z);
    env->get_element_by_name_t<gui_static_text>("2")->set_text(buf);

    /* Find out which face of the block we are looking at */

    int px = glm::floor(prevpos.x);
    int py = glm::floor(prevpos.y);
    int pz = glm::floor(prevpos.z);

    if(px > mx)
        face = 0;
    else if(px < mx)
        face = 3;
    else if(py > my)
        face = 1;
    else if(py < my)
        face = 4;
    else if(pz > mz)
        face = 2;
    else if(pz < mz)
        face = 5;

    /* If we are looking at air, move the cursor out of sight */

    if(chkmgr->GetBlock(glm::vec3(mx, my, mz)).GetBlockType()==EBT_DEFAULT)
    {
        mx = my = mz = 99999;
        validvoxel=false;
    }

    voxpos=glm::vec3(mx,my,mz);

    if(face == 0)
        mx++;
    if(face == 3)
        mx--;
    if(face == 1)
        my++;
    if(face == 4)
        my--;
    if(face == 2)
        mz++;
    if(face == 5)
        mz--;

    newvoxpos=glm::vec3(mx,my,mz);

    swprintf(buf,L"Face %d",face);
    env->get_element_by_name_t<gui_static_text>("3")->set_text(buf);

    swprintf(buf,L"VoxPos %.2f %.2f %.2f",voxpos.x,voxpos.y,voxpos.z);
    env->get_element_by_name_t<gui_static_text>("4")->set_text(buf);

    swprintf(buf,L"NewVoxPos %.2f %.2f %.2f",newvoxpos.x,newvoxpos.y,newvoxpos.z);
    env->get_element_by_name_t<gui_static_text>("5")->set_text(buf);
}

void VoxelzApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{
    if(action==GLFW_PRESS)
    {
        wchar_t buf[256];
        swprintf(buf,L"Total Chunks %d",chkmgr->GetChunkCount());
        env->get_element_by_name_t<gui_static_text>("6")->set_text(buf);

        swprintf(buf,L"Total Blocks %d",chkmgr->GetTotalBlocks());
        env->get_element_by_name_t<gui_static_text>("7")->set_text(buf);
        switch(button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            if(validvoxel)
                chkmgr->SetBlock(voxpos,EBT_DEFAULT,false);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if(validvoxel)
                chkmgr->SetBlock(newvoxpos,EBT_GRASS,true);
            break;
        }
    }
}

