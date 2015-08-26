#include "Precomp.h"

#include "Window.h"

std::map<GLFWwindow*, Window*>   Window::_windows;

void MouseMoveEvent(GLFWwindow * wnd, double x, double y)
{
    Window::_windows[wnd]->SigMouseMoved().emit(x,y);
}

void MouseKeyEvent(GLFWwindow * wnd, int32_t button, int32_t action, int32_t mod)
{
    Window::_windows[wnd]->SigMouseKey().emit(button,action,mod);
}

void MouseScrollEvent(GLFWwindow * wnd, double sx, double sy)
{
    Window::_windows[wnd]->SigMouseScroll().emit(sx,sy);
}

void WindowResize(GLFWwindow * wnd, int32_t w, int32_t h)
{
    Window::_windows[wnd]->SigWindowResized().emit(w,h);
}

void WindowClose(GLFWwindow * wnd)
{
    Window::_windows[wnd]->SigWindowClosed().emit();
    Window::_windows[wnd]->_shouldClose = true;
}

void KeyEvent(GLFWwindow * wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
    Window::_windows[wnd]->SigKeyEvent().emit(key,scan_code,action,modifiers);
}

void TextEvent(GLFWwindow * wnd, uint32_t scan_code)
{
    Window::_windows[wnd]->SigTextEvent().emit(scan_code);
}


sigc::signal<void, double, double> & Window::SigMouseMoved()
{
    return _sigMouseMoved;
}

sigc::signal<void, int32_t, int32_t, int32_t> & Window::SigMouseKey()
{
    return _sigMouseKey;
}

sigc::signal<void, double, double> & Window::SigMouseScroll()
{
    return _sigMouseScroll;
}

sigc::signal<void, int32_t, int32_t> & Window::SigWindowResized()
{
    return _sigWindowResized;
}

sigc::signal<void> & Window::SigWindowClosed()
{
    return _sigWindowClosed;
}

sigc::signal<void, int32_t, int32_t, int32_t, int32_t> & Window::SigKeyEvent()
{
    return _sigKeyEvent;
}

sigc::signal<void, uint32_t> & Window::SigTextEvent()
{
    return _sigTextEvent;
}

void Window::DestroyWindow(Window * wnd)
{
    delete wnd;
    Window::_windows.erase(wnd->GetWindow());
}

Window::Window()
{
    _window = nullptr;
    _shouldClose = false;
}

Window::~Window()
{
    _sigWindowResized.clear();
    _sigWindowClosed.clear();
    _sigMouseMoved.clear();
    _sigMouseKey.clear();
    _sigMouseScroll.clear();
    _sigKeyEvent.clear();
    _sigTextEvent.clear();

    glfwDestroyWindow(_window);
    glfwTerminate();
}

bool Window::Init(const std::string  &title, uint32_t width, uint32_t height, uint32_t r, uint32_t g, uint32_t b, uint32_t alpha, uint32_t depth, uint32_t stencil)
{
    if (!glfwInit())
    {
        printf("glfw init failed!\n"); //log
        return false;
    }

    //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE); //We don't want the old OpenGL

    glfwWindowHint(GLFW_RED_BITS, 8);
    glfwWindowHint(GLFW_GREEN_BITS, 8);
    glfwWindowHint(GLFW_BLUE_BITS, 8);
    glfwWindowHint(GLFW_ALPHA_BITS, 8);
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    glfwWindowHint(GLFW_RESIZABLE,0);


    /* Create a windowed mode Window and its OpenGL context */
    _window = glfwCreateWindow(width, height,title.c_str(),NULL,NULL);

    if (!_window)
    {
        printf("glfw open Window failed!\n"); //log
        return false;
    }

    _windows[_window]=this;
    glfwMakeContextCurrent(_window);

    glfwSetWindowSizeCallback(_window, &WindowResize);
    glfwSetWindowCloseCallback(_window,&WindowClose);
    glfwSetCursorPosCallback(_window, &MouseMoveEvent);
    glfwSetMouseButtonCallback(_window, &MouseKeyEvent);
    glfwSetScrollCallback(_window, &MouseScrollEvent);
    glfwSetKeyCallback(_window, &KeyEvent);
    glfwSetCharCallback(_window, &TextEvent);

    glfwSetWindowShouldClose(_window, GL_FALSE);

    //glfwSwapInterval(0);


    glfwSwapBuffers(_window);
    glfwPollEvents();
    return true;
}

bool Window::GetKey(uint32_t key)
{
    return glfwGetKey(_window,key);
}

glm::ivec2 Window::GetMousePos()
{
    double x, y;
    glfwGetCursorPos(_window,&x,&y);
    return glm::ivec2(x,y);
}

glm::ivec2 Window::GetWindowSize()
{
    int32_t x, y;
    glfwGetWindowSize(_window,&x,&y);
    return glm::ivec2(x,y);
}

void Window::SetMousePos(const glm::ivec2 & pos)
{
    glfwSetCursorPos(_window,pos.x,pos.y);
}

void Window::SetCursorDisabled(bool disabled)
{
    glfwSetInputMode(_window,GLFW_CURSOR,disabled?GLFW_CURSOR_DISABLED:GLFW_CURSOR_NORMAL);
}

GLFWwindow * Window::GetWindow()
{
    return _window;
}

bool Window::Update()
{
    glfwPollEvents();
    return _window!=nullptr;
}

void Window::SwapBuffers()
{
    glfwSwapBuffers(_window);
}

bool Window::GetShouldClose()
{
    return _shouldClose;
}
