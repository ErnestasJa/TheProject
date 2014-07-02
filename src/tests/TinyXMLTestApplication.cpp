#include "Precomp.h"

#include "Application/Window.h"
#include "utility/Timer.h"
#include "utility/Logger.h"
#include "opengl/OpenGLUtil.h"
#include "opengl/Texture.h"
#include "resources/ImageLoader.h"

#include "TinyXMLTestApplication.h"

#include "utility/Rect2d.h"
#include "gui/GUISkin.h"
#include "gui/GUIEnvironment.h"
#include "gui/GUIButton.h"
#include "gui/GUICheckbox.h"
#include "gui/GUIPane.h"
#include "gui/GUIEditBox.h"
#include "gui/GUIImage.h"

tinyxml_test_Application::tinyxml_test_Application(uint32_t argc, const char ** argv): Application(argc,argv)
{
}

tinyxml_test_Application::~tinyxml_test_Application()
{

}

bool tinyxml_test_Application::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);

    ///gl setup
    glClearColor(0.75f, 0.75f, 0.75f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    gui_skin s=gui_skin();
    s.load("res/skin_default.xml");

    env=new gui_environment(this->_window,this->GetLogger());

    gui_button* btn=new gui_button(env,rect2d<int>(0,0,64,64),L"HOLA");

    gui_checkbox* cb=new gui_checkbox(env,rect2d<int>(0,64,20,20),false);
    cb=new gui_checkbox(env,rect2d<int>(0,96,20,20),true);
    cb=new gui_checkbox(env,rect2d<int>(0,128,20,20),true);
    cb=new gui_checkbox(env,rect2d<int>(0,160,20,20),false);

    gui_edit_box* eb=new gui_edit_box(env,rect2d<int>(200,0,200,20),L"",glm::vec4(1,1,1,1),false,false);

    std::shared_ptr<texture> test_img=std::shared_ptr<texture>(new texture());
    image_loader* imgl=new image_loader(this->GetLogger());
    std::shared_ptr<image> img=std::shared_ptr<image>(imgl->load("res/skin_default.png"));
    test_img->init(img);

    gui_image* imagagae=new gui_image(env,rect2d<int>(400,0,256,256),test_img);

    return true;
}

bool tinyxml_test_Application::Update()
{
    if(_window->Update() && !_window->GetKey(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        _mainTimer->tick();
        env->update(0);
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        env->render();
        _window->swap_buffers();

        ///let's just rage quit on gl error
        return !this->_GLUtil->check_and_output_errors();
    }
    return false;
}

void tinyxml_test_Application::Exit()
{
    delete env;
    Application::Exit();
}
