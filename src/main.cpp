#include "precomp.h"
#include "tgaloader.h"
#include "texture.h"
#include "shader.h"
#include "iqmesh.h"
#include "iqmloader.h"
#include "quad.h"
#include "fbo.h"
#include "resource_cache.h"
#include "window.h"

static const char* gvs =
"#version 330\n"
"uniform mat4 MVP;\n"
"uniform mat3x4 bonemats[80];\n"
"layout (location=0) in vec3 pos;\n"
"layout (location=1) in vec2 tex;\n"
"layout (location=2) in vec3 normal;\n"
"layout (location=3) in vec4 vtangent;\n"
"layout (location=4) in vec4 vbones;\n"
"layout (location=5) in vec4 vweights;\n"
"out vec2 UV;\n"
"void main(void)\n"
"{\n"
"   mat3x4 m = bonemats[int(vbones.x)] * vweights.x;\n"
"   m += bonemats[int(vbones.y)] * vweights.y;\n"
"   m += bonemats[int(vbones.z)] * vweights.z;\n"
"   m += bonemats[int(vbones.w)] * vweights.w;\n"
"   vec4 mpos= vec4(vec4(pos,1)*m,1);\n"
"   gl_Position = MVP * mpos;\n"
"   //mat3 madjtrans = mat3(cross(m[1].xyz, m[2].xyz), cross(m[2].xyz, m[0].xyz), cross(m[0].xyz, m[1].xyz));\n"
"   //vec3 mnormal = normal * madjtrans;\n"
"   //vec3 mtangent = vtangent.xyz * madjtrans; // tangent not used, just here as an example\n"
"   //vec3 mbitangent = cross(mnormal, mtangent) * vtangent.w; // bitangent not used, just here as an example\n"
"}\n";

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
    if(!PHYSFS_exists(file.c_str()))
    {
        std::cout << "File \"" << file.c_str() << "\" not found." << std::endl;
        return NULL;
    }

    PHYSFS_file* f = PHYSFS_openRead(file.c_str());
    uint32_t length = PHYSFS_fileLength(f);

    char * buffer = new char[length];
    PHYSFS_read(f, buffer, 1, length);
    PHYSFS_close(f);

    tgaloader l;
    return l.generate(buffer,length);
}

void glerr()
{
    uint32_t err = GL_NO_ERROR;

    while((err=glGetError())!=GL_NO_ERROR)
    {
        std::cout<<"GL_ERROR: " << std::hex << err << std::endl;
    }
}

class resize_gl : public window_resize_callback
{
public:

    glm::mat4 * perspective;

    resize_gl(glm::mat4 * perspective_matrix)
    {
        perspective = perspective_matrix;
    }

    virtual void on_resize(int32_t w, int32_t h)
    {
        glViewport(0,0,w,h);
        perspective[0] = glm::perspective(45.f,4.f/3.f,0.1f,1000.f);
        std::cout<< std::endl <<"resized viewport"<< std::endl<< std::endl ;
    }
};

int main(int argc, const char ** argv)
{
    printf("Is this big endian platform? (%s)\n",is_big_endian()==true?"true":"false");


    window wnd;
    wnd.init(640,480);

    if (glxwInit()!=0)
    {
        std::cout<<"glxw_init() failed: " <<PHYSFS_getLastError()<<std::endl;
        return -1;
    }


    printf("OpenGL context version: %s\n",glGetString(GL_VERSION));

    /* Development going wild. or is it? */
    glClearColor(0.2f,0.2f,0.4f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    ///+++++++++++++++++++++++++++++++++++++

    if (!PHYSFS_init(argv[0]))
    {
        std::cout<<"PHYSFS_init() failed: " <<PHYSFS_getLastError()<<std::endl;
        return -1;
    }

    PHYSFS_addToSearchPath(PHYSFS_getBaseDir(), 1);

    std::cout<< "Search path: " << PHYSFS_getBaseDir() << std::endl;

    if(!PHYSFS_exists("res/mrfixit.iqm"))
    {
        std::cout << "File \"res/mrfixit.iqm\" not found." << std::endl;
        return -1;
    }

    PHYSFS_file* f = PHYSFS_openRead("res/mrfixit.iqm");

    char * buffer = new char[PHYSFS_fileLength(f)];

    PHYSFS_read(f, buffer, 1, PHYSFS_fileLength(f));
    PHYSFS_close(f);

    iqmloader *loader=new iqmloader();
    iqmesh * mesh=loader->load(buffer);
    loader->loadiqmanims(mesh);

    mesh->generate();
    ///-------------------------------------


    ///+++++++++++++++++++++++++++++++++++++

    quad q;
    q.generate();

    binding qtex_binding[]={{"tex",0},{"",-1}};
    shader qsh("default",quad_vs_textured,quad_fs_textured,qtex_binding,0);
	qsh.compile();
	qsh.link();

    ///-------------------------------------

    glm::mat4 M=glm::mat4(1.0f);
    glm::mat4 V=glm::lookAt(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0));
    glm::mat4 P=glm::perspective(45.f,4.f/3.f,0.1f,1000.f);
    M=glm::translate(M,glm::vec3(-10,5,-10));
    M=glm::scale(M,glm::vec3(0.01,0.01,0.01));
    V = glm::rotate<float>(V,-90,glm::vec3(0,1,0));
    glm::mat4 MVP;

    shader sh("default",gvs,fs,0,0);
	sh.compile();
	sh.link();

    double lastTime = glfwGetTime();
    int32_t nbFrames = 0;

    ///lets render to texture, huh?
    auto * cache = create_resource_cache<texture>();
    auto tex = share(new texture());
    std::cout << "Use count:" << tex.use_count() << std::endl;

    cache->push_back(tex);
    tex->generate(NULL,GL_TEXTURE_2D,32,1024,1024);

    fbo fb;
    fb.generate();
    fb.set(GL_FRAMEBUFFER);
    fb.attach(GL_COLOR_ATTACHMENT0,tex);
    std::cout << "Use count:" << tex.use_count() << std::endl;

    if(fb.is_complete())
        std::cout<<"FBO is complete.\n";
    else
        std::cout<<"FBO is not complete.\n";

    int32_t max_attachments; glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS,&max_attachments);

    std::cout << "Max color attachments:" << max_attachments << std::endl;

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    mesh->set_frame(80);

    MVP=P*V*M;

    sh.set();
    glUniformMatrix4fv(sh.getparam("MVP"),1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix3x4fv(sh.getparam("bonemats"),mesh->data_header.num_joints,GL_FALSE,&mesh->current_frame[0][0].x);
    mesh->draw(false);

    //fb.detach(GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D);
    fb.unset();


    ///----------------------------


    resize_gl rsz(&P);
    wnd.resize_callback = std::bind(&resize_gl::on_resize, &rsz, std::placeholders::_1, std::placeholders::_2);;

    /* Loop until the user closes the window */
    while (wnd.update())
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

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        qsh.set();
        tex->set(0);
        q.draw();

        //glerr();

        /* Swap front and back buffers and process events */
        wnd.swap_buffers();
    }

    tex=NULL;

    delete mesh;
    delete loader;

    delete cache;
    return 0;
}
