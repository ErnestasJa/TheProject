#include "Precomp.h"
#include "CollisionTestApp.h"

#include "physics/CollisionObject.h"
#include "voxel/VoxelTypes.h"

CollisionTestApp::CollisionTestApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

CollisionTestApp::~CollisionTestApp()
{

}

CollisionObject *one, *two;

bool CollisionTestApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.5,0.5,0.7,0);

    one=new CollisionObject(glm::vec3(0.9f,0.9f,0),glm::vec3(0.5f));
    two=new CollisionObject(glm::vec3(0,0,0),glm::vec3(0.5f));

    bool res=true;//MPRCollide(one,two);
    printf("Collision result: %s\n%s\n%s\n",res?"true":"false",GLMVec3ToStr(CCDtoGLM(one->_pos)),GLMVec3ToStr(CCDtoGLM(two->_pos)));
    //CollisionInfo colinf=MPRPenetration(one,two);
    //printf("Collision info:\nColliding:%s\nDepth:%f\nPos: %s\nDir: %s\n",colinf.colliding?"true":"false",colinf.depth,GLMVec3ToStr(CCDtoGLM(colinf.pos)),GLMVec3ToStr(CCDtoGLM(colinf.dir)));

    return true;
}

bool CollisionTestApp::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        return true;
    }
    return false;
}

void CollisionTestApp::Exit()
{
    Application::Exit();
}

void CollisionTestApp::OnWindowClose()
{

}
void CollisionTestApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
}

void CollisionTestApp::OnMouseMove(double x, double y)
{

}

void CollisionTestApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{

}

