#include "Precomp.h"
#include "TemplateApp.h"

TemplateApp::TemplateApp(uint32_t argc, const char ** argv): Application(argc,argv)
{

}

TemplateApp::~TemplateApp()
{

}

bool TemplateApp::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glClearColor(0.5,0.5,0.7,0);

    return true;
}

bool TemplateApp::Update()
{
    if(_appContext->_window->Update() && !_appContext->_window->GetShouldClose() && !_appContext->_window->GetKey(GLFW_KEY_ESCAPE))
    {
        return true;
    }
    return false;
}

void TemplateApp::Exit()
{
    Application::Exit();
}

void TemplateApp::OnWindowClose()
{

}
void TemplateApp::OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
}

void TemplateApp::OnMouseMove(double x, double y)
{

}

void TemplateApp::OnMouseKey(int32_t button, int32_t action, int32_t mod)
{

}

