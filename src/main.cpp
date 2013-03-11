#include <stdio.h>

int main()
{
	printf("Hello world!\n");

	glxwInit();
	
	if (!glfwInit())
	{
		printf("glfw init failed!\n");
        return -1;
	}

    /* Create a windowed mode window and its OpenGL context */
    if (!glfwOpenWindow(640, 480, 8, 8, 8, 0, 24, 0, GLFW_WINDOW))
	{
		printf("glfw open window failed!\n");
        return -1;
	}
	
	/* Development going wild. or is it? */
	glClearColor(0.2f,0.2f,0.4f,1.0f);
	
    /* Loop until the user closes the window */
    while (glfwGetWindowParam(GLFW_OPENED)&&!glfwGetKey(GLFW_KEY_ESC))
    {
        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT);
        /* Swap front and back buffers and process events */
        glfwSwapBuffers();
    }

	return 0;
}