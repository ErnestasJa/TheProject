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
	//seek to the end of file
	fseek(io,0,SEEK_END);
	//get the size
	int fsize=ftell(io);
	printf("Filesize ftell:%i\n",fsize);
	//get back to the beginning
	fseek(io,SEEK_SET,0);
	
	//read the header
	fread(&head,sizeof(head),1,io);
	
	//size check works
	printf("Header size:%i\nFile size:%i\nSize check:%s\n",(int)sizeof(head),head.filesize,(int)head.filesize==fsize?"same":"not same");
	
	iqmesh *mesh;
	
	iqmloader *loader=new iqmloader();
	
	uint8_t* buffer=new uint8_t[head.filesize];
	fread(buffer+sizeof(head),head.filesize-sizeof(head),1,io);
	
	mesh=loader->load(buffer,head);
	mesh->generate();
	glEnable(GL_DEPTH);
	glDepthFunc(GL_LESS);
    /* Loop until the user closes the window */
    while (glfwGetWindowParam(GLFW_OPENED)&&!glfwGetKey(GLFW_KEY_ESC))
    {
        /* Render here */
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		
        /* Swap front and back buffers and process events */
        glfwSwapBuffers();
    }

	return 0;
}