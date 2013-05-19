#pragma once

#include "precomp.h"
#include "GL/glfw.h"

#include <functional>

class window_resize_callback
{
public:
    virtual void on_resize(int32_t w, int32_t h)=0;
};

class window
{
protected:

public:
    window()
    {

    }

    static std::function< void(int32_t,int32_t) > resize_callback;

    static void GLFWCALL on_resize( int32_t width, int32_t height )
    {
        if(resize_callback)
            resize_callback(width, height);
    }

    bool init(const std::string  &title, uint32_t width, uint32_t height, uint32_t r=8, uint32_t g=8, uint32_t b=8, uint32_t alpha=8, uint32_t depth=24, uint32_t stencil=8)
    {
        if (!glfwInit())
        {
            printf("glfw init failed!\n"); //log
            return false;
        }

        //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // We want OpenGL 3.3
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

        /* Create a windowed mode window and its OpenGL context */
        if (!glfwOpenWindow(width, height, r, g, b, alpha, depth, stencil, GLFW_WINDOW))
        {
            printf("glfw open window failed!\n"); //log
            return false;
        }

        glfwSetWindowTitle(title.c_str());

        glfwSetWindowSizeCallback(&window::on_resize);

        return true;
    }

    bool update()
    {
        glfwPollEvents();
        return glfwGetWindowParam(GLFW_OPENED);
    }

    void swap_buffers()
    {
        glfwSwapBuffers();
    }

};
