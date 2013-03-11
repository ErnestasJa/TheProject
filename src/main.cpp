#include "iqmesh.h"
#include "iqmloader.h"
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
	
	FILE* io=fopen("../../ZombieGameProject/res/mrfixit.iqm","rb");
	
	iqmheader head;
	fread(&head,sizeof(head),1,io);
	printf("Header size:%i\nFile size:%i\n",sizeof(head),head.filesize);
	iqmesh *mesh;
	
	iqmloader *loader=new iqmloader();
	
	uint8_t* buffer=new uint8_t[head.filesize];
	fread(buffer+sizeof(head),head.filesize-sizeof(head),1,io);
	
	mesh=loader->load(buffer,head);
	
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