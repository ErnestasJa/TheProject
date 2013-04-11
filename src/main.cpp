#include "precomp.h"
#include "tgaloader.h"
#include "texture.h"
#include "shader.h"
#include "iqmesh.h"
#include "iqmloader.h"

static const char* vs = R"(
#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 tex_coords;
uniform mat4 MVP;
out vec2 UV;
void main()
{
    UV=tex_coords;
    gl_Position = MVP*vec4(position.x, position.y, position.z, 1.0);
})";

static const char* fs = R"(
#version 330
uniform sampler2D tex;
in vec2 UV;
out vec4 FragColor;

void main()
{
    FragColor = texture2D(tex,UV);
})";

static bool is_big_endian(void)
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

texture * load_tex(std::string file)
{
    std::ifstream rfile(file);

    if(!rfile.is_open())
    {
        printf("Could not open file\n");
        return NULL;
    }

    rfile.seekg(0, std::ios::end);
    uint32_t size = rfile.tellg();
    rfile.seekg(0);


    tgaloader l;
    char * buf = new char[size];
    rfile.read(buf,size);
    return l.generate(buf,size);
}

int main()
{
    printf("sizeof(iqmanim)=%lu\n", sizeof(iqmanim));
    printf("Is this bitch big endian? (%s)\n",is_big_endian()==true?"true":"false");

    if (!glfwInit())
    {
        printf("glfw init failed!\n");
        return -1;
    }

    glfwOpenWindowHint(GLFW_FSAA_SAMPLES, 4); // 4x antialiasing
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3); // We want OpenGL 3.3
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL

    /* Create a windowed mode window and its OpenGL context */
    if (!glfwOpenWindow(800, 600, 8, 8, 8, 0, 24, 0, GLFW_WINDOW))
    {
        printf("glfw open window failed!\n");
        return -1;
    }

    if(glxwInit()!=0)
    {
        printf("GLXW failed to initialize.\n");
        return -1;
    }

    printf("OpenGL context version: %s\n",glGetString(GL_VERSION));

    /* Development going wild. or is it? */
    glClearColor(0.2f,0.2f,0.4f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    FILE* io=fopen("../../ZombieGame/res/mrfixit.iqm","rb");

    iqmheader head;
    //seek to the end of file
    fseek(io,0,SEEK_END);
    //get the size
    int32_t fsize=ftell(io);
    printf("Filesize ftell:%i\n",fsize);
    //get back to the beginning
    fseek(io,SEEK_SET,0);

    //read the header
    fread(&head,sizeof(head),1,io);

    //size check works
    printf("Header size:%i\nFile size:%i\nSize check:%s\n",(int32_t)sizeof(head),head.filesize,(int32_t)head.filesize==fsize?"same":"not same");

    iqmesh *mesh;

    iqmloader *loader=new iqmloader();

    uint8_t* buffer=new uint8_t[head.filesize];
    fread(buffer+sizeof(head),head.filesize-sizeof(head),1,io);
    fclose(io);

    mesh=loader->load(buffer,head);
    delete[] buffer;


    double lastTime = glfwGetTime();
    int32_t nbFrames = 0;

    binding tex_binding[]={{"tex",0},{"",-1}};
    binding attrib_binding[]={{"position",0},{"tex_coords",1},{"",-1}};
    shader sh("default vertex shader",vs,fs,attrib_binding,tex_binding);
	sh.compile();
	sh.link();
	sh.set();

    glm::mat4 M=glm::mat4(1.0f);
    glm::mat4 V=glm::lookAt(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0));
    glm::mat4 P=glm::perspective(45.f,4.f/3.f,0.1f,1000.f);
    glm::mat4 MVP=P*V*M;

    GLuint mpl = sh.getparam("MVP");
    glUniformMatrix4fv(mpl,1,GL_FALSE,glm::value_ptr(MVP));

    texture * tex = load_tex("../../ZombieGame/res/Body.tga");
    texture * tex2 = load_tex("../../ZombieGame/res/Head.tga");

    mesh->submeshes[0].mat.texid=tex->obj;
    mesh->submeshes[1].mat.texid=tex2->obj;

    printf("Animations: %i %i\n",mesh->anims[0].first_frame,mesh->anims[0].num_frames);

    /* Loop until the user closes the window */
    while (glfwGetWindowParam(GLFW_OPENED)&&!glfwGetKey(GLFW_KEY_ESC))
    {
        // Measure speed
        double currentTime = glfwGetTime();
        nbFrames++;
        if ( currentTime - lastTime >= 1.0 )  // If last prinf() was more than 1 sec ago
        {
            // printf and reset timer
            printf("FPS: %i (%f ms/frame)\n",nbFrames,1000.0/double(nbFrames));
            nbFrames = 0;
            lastTime += 1.0;
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        M=glm::rotate<float>(M,1,glm::vec3(0,1,0));
        MVP=P*V*M;
        glUniformMatrix4fv(mpl,1,GL_FALSE,glm::value_ptr(MVP));
        mesh->draw(false);

        /* Swap front and back buffers and process events */
        glfwSwapBuffers();
    }
    delete mesh;
    delete loader;
    return 0;
}
