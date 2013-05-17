#pragma once

#include <GL/glfw.h>
class window
{
protected:

public:
    window()
    {

    }

    bool init(uint32_t width, uint32_t height, uint32_t r=8, uint32_t g=8, uint32_t b=8, uint32_t alpha=8, uint32_t depth=24, uint32_t stencil=8)
    {
        if (!glfwInit())
        {
            printf("glfw init failed!\n"); //log
            return -1;
        }

        //glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // We want OpenGL 3.3
        glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
        glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

        /* Create a windowed mode window and its OpenGL context */
        if (!glfwOpenWindow(width, height, r, g, b, alpha, depth, stencil, GLFW_WINDOW))
        {
            printf("glfw open window failed!\n"); //log
            return -1;
        }
    }

    bool update()
    {
        return glfwGetWindowParam(GLFW_OPENED);
    }

    void swap_buffers()
    {
        glfwSwapBuffers();
    }

};
