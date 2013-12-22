#pragma once

#include "precomp.h"

#include "GLFW/glfw3.h"

#include <functional>
#include <map>

class window_resize_callback
{
public:
    virtual void on_resize(int32_t w, int32_t h)=0;
};

class window
{
protected:
    GLFWwindow * m_window;

    static std::map<GLFWwindow*, window*> m_windows;

    static void on_close(GLFWwindow * w)
    {
        m_windows[w]->m_window = nullptr;
    }

    static void on_resize(GLFWwindow * w, int32_t width, int32_t height )
    {
        if(resize_callback)
            resize_callback(width, height);
    }

    static std::function< void(int32_t,int32_t) > resize_callback;

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
        glfwWindowHint(GLFW_DEPTH_BITS, 24);

        /* Create a windowed mode window and its OpenGL context */
        m_window = glfwCreateWindow(width, height,title.c_str(),NULL,NULL);

        if (!m_window)
        {
            printf("glfw open window failed!\n"); //log
            return false;
        }

        glfwMakeContextCurrent(m_window);

        glfwSetWindowSizeCallback(m_window, &window::on_resize);
        glfwSetWindowCloseCallback(m_window,&window::on_close);

        m_windows[m_window]=this;
        return true;
    }

    bool get_key(uint32_t key)
    {
        return glfwGetKey(m_window,key);
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
