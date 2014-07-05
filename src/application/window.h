#pragma once

void KeyEvent(GLFWwindow * wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
void MouseMoveEvent(GLFWwindow * wnd, double x, double y);
void MouseKeyEvent(GLFWwindow * wnd, int32_t button, int32_t action, int32_t mod);
void MouseScrollEvent(GLFWwindow * wnd, double sx, double sy);
void WindowResize(GLFWwindow * wnd, int32_t w, int32_t h);
void WindowClose(GLFWwindow * wnd);
void TextEvent(GLFWwindow * wnd, uint32_t scan_code);

class Window
{
protected:
    GLFWwindow * _window;

    sigc::signal<void, double, double> _sigMouseMoved;
    sigc::signal<void, int32_t, int32_t, int32_t> _sigMouseKey;
    sigc::signal<void, double, double> _sigMouseScroll;
    sigc::signal<void, int32_t, int32_t> _sigWindowResized;
    sigc::signal<void> _sigWindowClosed;
    sigc::signal<void, int32_t, int32_t, int32_t, int32_t> _sigKeyEvent;
    sigc::signal<void, uint32_t > _sigTextEvent;

    friend void MouseMoveEvent(GLFWwindow * wnd, double x, double y);
    friend void MouseKeyEvent(GLFWwindow * wnd, int32_t button, int32_t action, int32_t mod);
    friend void MouseScrollEvent(GLFWwindow * wnd, double sx, double sy);
    friend void WindowResize(GLFWwindow * wnd, int32_t x, int32_t y);
    friend void WindowClose(GLFWwindow * wnd);
    friend void KeyEvent(GLFWwindow * wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
    friend void TextEvent(GLFWwindow * wnd, uint32_t scan_code);

    bool shouldClose;
public:
    static std::map<GLFWwindow*, Window*> _windows;
    static void DestroyWindow(Window * wnd);

    Window()
    {
        _window = nullptr;
        shouldClose = false;
    }

    ~Window()
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

    bool Init(const std::string  &title, uint32_t width, uint32_t height, uint32_t r=8, uint32_t g=8, uint32_t b=8, uint32_t alpha=8, uint32_t depth=24, uint32_t stencil=8)
    {
        if (!glfwInit())
        {
            printf("glfw init failed!\n"); //log
            return false;
        }

        //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

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

        glfwMakeContextCurrent(_window);

        glfwSetWindowSizeCallback(_window, &WindowResize);
        glfwSetWindowCloseCallback(_window,&WindowClose);
        glfwSetCursorPosCallback(_window, &MouseMoveEvent);
        glfwSetMouseButtonCallback(_window, &MouseKeyEvent);
        glfwSetScrollCallback(_window, &MouseScrollEvent);
        glfwSetKeyCallback(_window, &KeyEvent);
        glfwSetCharCallback(_window, &TextEvent);

        glfwSetWindowShouldClose(_window, GL_FALSE);

        _windows[_window]=this;
        return true;
    }

    bool GetKey(uint32_t key)
    {
        return glfwGetKey(_window,key);
    }

    glm::ivec2 GetMousePos()
    {
        double x, y;
        glfwGetCursorPos(_window,&x,&y);
        return glm::ivec2(x,y);
    }

    glm::ivec2 GetWindowSize()
    {
        int32_t x, y;
        glfwGetWindowSize(_window,&x,&y);
        return glm::ivec2(x,y);
    }

    void set_mouse_pos(const glm::ivec2 & pos)
    {
        glfwSetCursorPos(_window,pos.x,pos.y);
    }

    void set_cursor_disabled(bool disabled)
    {
        glfwSetInputMode(_window,GLFW_CURSOR,disabled?GLFW_CURSOR_DISABLED:GLFW_CURSOR_NORMAL);
    }

    GLFWwindow *getWindow()
    {
        return _window;
    }

    sigc::signal<void, double, double> & SigMouseMoved()
    {
        return _sigMouseMoved;
    }

    sigc::signal<void, int32_t, int32_t, int32_t> & SigMouseKey()
    {
        return _sigMouseKey;
    }

    sigc::signal<void, double, double> & SigMouseScroll()
    {
        return _sigMouseScroll;
    }

    sigc::signal<void, int32_t, int32_t> & SigWindowResized()
    {
        return _sigWindowResized;
    }

    sigc::signal<void> & SigWindowClosed()
    {
        return _sigWindowClosed;
    }

    sigc::signal<void, int32_t, int32_t, int32_t, int32_t> & SigKeyEvent()
    {
        return _sigKeyEvent;
    }

    sigc::signal<void, uint32_t> & SigTextEvent()
    {
        return _sigTextEvent;
    }

    bool Update()
    {
        glfwPollEvents();
        return _window!=nullptr;
    }

    void swap_buffers()
    {
        glfwSwapBuffers(_window);
    }

    bool getShouldClose()
    {
        return shouldClose;
    }

};
