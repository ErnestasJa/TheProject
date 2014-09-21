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
#include "resources/ImageLoader.h"
#include "Opengl/CubeMesh.h"
#include "Opengl/GridMesh.h"
#include "Voxel/Block.h"
#include "Voxel/Chunk.h"
#include "Voxel/ChunkManager.h"
#include "Voxel/VoxelSprite.h"
#include "GUI/GUI.h"
#include "GUI/custom_elements/GUIColorPicker.h"

VoxelzApp::VoxelzApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

VoxelzApp::~VoxelzApp()
{

}

static MeshPtr mesh;
static ShaderPtr sh,vsh,qsh,gbsh,ssaosh;
static CameraPtr cam;
static CubeMesh *cub,*smallcub;
static GridMesh *grid;
static ChunkManager *chkmgr;
static GUIEnvironment *env;
static gui_image *guiImg;
static FrameBufferObject* GBuffer;
static TexturePtr GBdepth,GBdiffuse,GBnormal,GBposition,GBtexcoord,SSAONormal;
static glm::vec3 voxpos,newvoxpos,pointpos;
static bool validvoxel,wireframe;
static int face;
static VoxelSprite *spr;

//static glm::vec3 rgb2hsv(float r, float g, float b)
//{
//    float h,s,v;
//
//    float min, max, delta;
//    min = MIN( r, g, b );
//    max = MAX( r, g, b );
//    v = max;				// v
//    delta = max - min;
//    if( max != 0 )
//        s = delta / max;		// s
//    else
//    {
//        // r = g = b = 0		// s = 0, v is undefined
//        s = 0;
//        h = -1;
//        return;
//    }
//    if( r == max )
//        h = ( g - b ) / delta;		// between yellow & magenta
//    else if( g == max )
//        h = 2 + ( b - r ) / delta;	// between cyan & yellow
//    else
//        h = 4 + ( r - g ) / delta;	// between magenta & cyan
//    h *= 60;				// degrees
//    if( h < 0 )
//        h += 360;
//
//    return glm::vec3(h,s,v);
//}

bool InitPostProc(AppContext* ctx)
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

    SSAONormal=share(new Texture());
    image_loader* loader=new image_loader(ctx->_logger);
    std::shared_ptr<image> img=std::shared_ptr<image>(loader->load("res/SSAO_noise.png"));
    SSAONormal->Init(nullptr,GL_TEXTURE_2D,GL_RGB,GL_RGB,1280,768);
    uint32_t sx=1280/64;
    uint32_t sy=768/64;

    loop(x,sx)
    loop(y,sy)
    SSAONormal->SetSubImage2D(img->data,x*64,y*64,64,64);

#define _DEBUG_FBO
#ifdef DEBUG_FBO
    guiImg=new gui_image(env,Rect2D<int>(1280-320,0,320,192),GBdepth,false);
    guiImg=new gui_image(env,Rect2D<int>(1280-640,0,320,192),GBdiffuse);
    guiImg=new gui_image(env,Rect2D<int>(1280-320,192,320,192),GBnormal);
    guiImg=new gui_image(env,Rect2D<int>(1280-320,384,320,192),GBposition);
    guiImg=new gui_image(env,Rect2D<int>(1280-320,576,320,192),GBtexcoord);
    guiImg=new gui_image(env,Rect2D<int>(1280-640,192,320,192),SSAONormal);
#endif

//    gui_window *twin=new gui_window(env,Rect2D<int>(0,0,512,512),L"Voxelmator3000",true,false,false,true);
//
//    GUIColorPicker* cp=new GUIColorPicker(env,Rect2D<int>(0,16,128,128),false);
//    cp->SetParent(twin);

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

    spr=VoxelSprite::LoadFromImage(loader->load("res/mewtwo.png"),8,u8vec4(0));

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
    vsh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/voxel");
    qsh = (new shader_loader(ctx->_logger))->load("res/quad");
    gbsh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/gbuffer");
    ssaosh = (new shader_loader(ctx->_logger))->load("res/engine/shaders/SSAO");

    cam=share(new Camera(ctx,glm::vec3(0,128,0),glm::vec3(0,128,32),glm::vec3(0,1,0)));
    //cam->SetFPS(false);

    env=new GUIEnvironment(ctx);
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

    ctx->_timer->tick();
    chkmgr=new ChunkManager();
    ctx->_timer->tick();
    printf("\n\nBuilding took: %d ms\n\n\n",ctx->_timer->get_delta_time());
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

    InitPlaneMesh(_appContext);;
    if(!InitPostProc(_appContext))
        return false;
    return true;
}

bool VoxelzApp::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        _appContext->_timer->tick();

        cam->Update(0);
        //GBuffer->Set();
        //GBuffer->EnableBuffer(0);
        //GBuffer->EnableBuffer(1);
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

        vsh->Set();
        Model = glm::mat4(1.0f);
        MVP   = cam->GetViewProjMat() * Model;
        MVar<glm::mat4>(0, "mvp", MVP).Set();
        spr->Render();

        gbsh->Set();
        Model = glm::mat4(1.0f);
        MVP   = cam->GetViewProjMat() * Model;
        if(gbsh->getparam("v_inv")!=-1)
            MVar<glm::mat4>(gbsh->getparam("v_inv"), "v_inv", glm::inverse(cam->GetViewMat())).Set();
        MVar<glm::mat4>(gbsh->getparam("P"), "P", cam->GetProjectionMat()).Set();
        MVar<glm::mat4>(gbsh->getparam("V"), "V", cam->GetViewMat()).Set();
        chkmgr->Render(cam.get(),gbsh);
        //GBuffer->Unset();

        /// RENDER TO QUAD
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        ssaosh->Set();
//
//        GBdiffuse->Set(0);
//        if(ssaosh->getparam("g_buffer_diff")!=-1) MVar<int32_t>(ssaosh->getparam("g_buffer_diff"), "g_buffer_diff", 0).Set();
//        GBnormal->Set(1);
//        if(ssaosh->getparam("g_buffer_norm")!=-1) MVar<int32_t>(ssaosh->getparam("g_buffer_norm"), "g_buffer_norm", 1).Set();
//        GBposition->Set(2);
//        if(ssaosh->getparam("g_buffer_pos")!=-1) MVar<int32_t>(ssaosh->getparam("g_buffer_pos"), "g_buffer_pos", 2).Set();
//        SSAONormal->Set(3);
//        if(ssaosh->getparam("g_random")!=-1) MVar<int32_t>(ssaosh->getparam("g_random"), "g_random", 3).Set();
//        GBdepth->Set(4);
//        if(ssaosh->getparam("g_depth")!=-1) MVar<int32_t>(ssaosh->getparam("g_depth"), "g_depth", 4).Set();
//
//        if(ssaosh->getparam("P")!=-1) MVar<glm::mat4>(ssaosh->getparam("P"), "P", cam->GetProjectionMat()).Set();
//        if(ssaosh->getparam("MV")!=-1) MVar<glm::mat4>(ssaosh->getparam("MV"), "MV", cam->GetViewMat()*glm::mat4(1)).Set();

        //mesh->Render();

        env->Render();

        glDisable(GL_DEPTH_TEST);
        env->get_font_renderer()->render_string_formatted(L"multiline\nyadda yadda\nsuch wow\nlietuviškai,broli",glm::vec2(0,256),200,true);
        glEnable(GL_DEPTH_TEST);
        _appContext->_window->SwapBuffers();
        return true;
    }
    return false;
}

void VoxelzApp::Exit()
{
    delete chkmgr;
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
        if(chkmgr->GetBlock(glm::vec3(mx, my, mz)).GetBlockType()!=EBT_AIR)
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

    if(chkmgr->GetBlock(glm::vec3(mx, my, mz)).GetBlockType()==EBT_AIR)
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
                chkmgr->SetBlock(voxpos,EBT_AIR,false);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            if(validvoxel)
                chkmgr->SetBlock(newvoxpos,EBT_GRASS,true);
            break;
        }
    }
}

