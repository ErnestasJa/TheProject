#pragma once

void key_event(GLFWwindow * wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
void mouse_move(GLFWwindow * wnd, double x, double y);
void window_resize(GLFWwindow * wnd, int32_t w, int32_t h);
void window_close(GLFWwindow * wnd);

class window
{
protected:
    static std::map<GLFWwindow*, window*> m_windows;
    GLFWwindow * m_window;

    sigc::signal<void, double, double> m_sig_mouse_moved;
    sigc::signal<void, int32_t, int32_t> m_sig_window_resized;
    sigc::signal<void, window*> m_sig_window_closed;
    sigc::signal<void, int32_t, int32_t, int32_t, int32_t> m_sig_key_event;

    friend void mouse_move(GLFWwindow * wnd, double x, double y);
    friend void window_resize(GLFWwindow * wnd, int32_t x, int32_t y);
    friend void window_close(GLFWwindow * wnd);
    friend void key_event(GLFWwindow * wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);

public:

    window()
    {
        m_window = nullptr;
    }

    bool init(const std::string  &title, uint32_t width, uint32_t height, uint32_t r=8, uint32_t g=8, uint32_t b=8, uint32_t alpha=8, uint32_t depth=24, uint32_t stencil=8)
    {
        if (!glfwInit())
        {
            printf("glfw init failed!\n"); //log
            return false;
        }

        //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

        glfwWindowHint(GLFW_RED_BITS, 8);
        glfwWindowHint(GLFW_GREEN_BITS, 8);
        glfwWindowHint(GLFW_BLUE_BITS, 8);
        glfwWindowHint(GLFW_ALPHA_BITS, 8);
        glfwWindowHint(GLFW_DEPTH_BITS, 24);

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(width, height,title.c_str(),NULL,NULL);

        if (!m_window)
        {
            printf("glfw open window failed!\n"); //log
            return false;
        }

        glfwMakeContextCurrent(m_window);

        glfwSetWindowSizeCallback(m_window, &window_resize);
        glfwSetWindowCloseCallback(m_window,&window_close);
        glfwSetCursorPosCallback(m_window, &mouse_move);
        glfwSetKeyCallback(m_window, &key_event);

        m_windows[m_window]=this;
        return true;
    }

    bool get_key(uint32_t key)
    {
        return glfwGetKey(m_window,key);
    }

    glm::ivec2 get_mouse_pos()
    {
        double x, y;
        glfwGetCursorPos(m_window,&x,&y);
        return glm::ivec2(x,y);
    }

    glm::ivec2 get_window_size()
    {
        int32_t x, y;
        glfwGetWindowSize(m_window,&x,&y);
        return glm::ivec2(x,y);
    }

    void set_mouse_pos(const glm::ivec2 & pos)
    {
        glfwSetCursorPos(m_window,pos.x,pos.y);
    }

    void set_cursor_disabled(bool disabled)
    {
        glfwSetInputMode(m_window,GLFW_CURSOR,disabled?GLFW_CURSOR_DISABLED:GLFW_CURSOR_NORMAL);
    }

    GLFWwindow *getWindow()
    {
        return m_window;
    }

    sigc::signal<void, double, double> & sig_mouse_moved()
    {
        return m_sig_mouse_moved;
    }

    sigc::signal<void, double, double> & sig_window_resized()
    {
        return m_sig_mouse_moved;
    }

    sigc::signal<void, window*> & sig_window_closed()
    {
        return m_sig_window_closed;
    }

    sigc::signal<void, int32_t, int32_t, int32_t, int32_t> & sig_key_event()
    {
        return m_sig_key_event;
    }

    bool update()
    {
        glfwPollEvents();
        return m_window!=nullptr;
    }

    void swap_buffers()
    {
        glfwSwapBuffers(m_window);
    }
};
