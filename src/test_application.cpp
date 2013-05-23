#include "test_application.h"

#include "iqmloader.h"
#include "window.h"
#include "timer.h"
#include "logger.h"
#include "quad.h"

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

test_application::test_application(uint32_t argc, const char ** argv): application(argc,argv)
{

}

test_application::~test_application()
{

}

bool test_application::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);
    mesh_cache = create_resource_cache<iqmesh>();
    shader_cache = create_resource_cache<shader>();
    tex_cache = create_resource_cache<texture>();
    fbo_cache = create_resource_cache<frame_buffer_object>();

    frame_count = 0;
    last_time = 0;

    ///load our mesh
    PHYSFS_file* f = PHYSFS_openRead("res/mrfixit.iqm");

    if(!f)
        return false;

    char * buffer = new char[PHYSFS_fileLength(f)];

    PHYSFS_read(f, buffer, 1, PHYSFS_fileLength(f));
    PHYSFS_close(f);

    iqmloader *loader=new iqmloader();

    iqmesh * mesh=loader->load(buffer);
    if(mesh)
    {
        loader->loadiqmanims(mesh);
        mesh->generate();
        mesh_cache->push_back(share(mesh));
    }

    delete loader;

    ///prepare shaders, texture, fbo and quad

    binding qtex_binding[]={{"tex",0},{"",-1}};
    qsh = new shader("quad_shader",quad_vs_textured,quad_fs_textured,qtex_binding,0);
	qsh->compile();
	qsh->link();
	shader_cache->push_back(share(qsh));

	sh = new shader("gpu_skin",gvs,fs,0,0);
	sh->compile();
	sh->link();
	shader_cache->push_back(share(sh));

	tex = new texture();
	auto shared_tex = share(tex);
    tex->generate(NULL,GL_TEXTURE_2D,32,1024,1024);
    tex_cache->push_back(shared_tex);

    fbo = new frame_buffer_object();
    fbo->generate();
    fbo->set(GL_FRAMEBUFFER);
    fbo->attach(GL_COLOR_ATTACHMENT0,shared_tex);

    if(fbo->is_complete()) /// log this..
        m_log->log(LOG_DEBUG,"FBO is complete.");
        //std::cout<<"FBO is complete.\n";
    else
        m_log->log(LOG_DEBUG,"FBO is not complete.");
       //std::cout<<"FBO is not complete.\n";

    fbo_cache->push_back(share(fbo));

    q = new quad();
    q->generate();

    ///set up matrices

    M = glm::mat4(1.0f);
    M = glm::translate(M,glm::vec3(-10,5,-10));
    M = glm::scale(M,glm::vec3(0.01,0.01,0.01));

    V = glm::lookAt(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0));
    V = glm::rotate<float>(V,-90,glm::vec3(0,1,0));

    P = glm::perspective(45.f,4.f/3.f,0.1f,1000.f);

    MVP=P*V*M;

    ///gl setup
    glClearColor(0.0f, 0.0f, 0.568f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    ///render to fbo
    sh->set();
    glUniformMatrix4fv(sh->getparam("MVP"),1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix3x4fv(sh->getparam("bonemats"),mesh->data_header.num_joints,GL_FALSE,&mesh->current_frame[0][0].x);

    mesh->set_frame(80);
    mesh->draw(false);

    fbo->unset();



    return true;
}

bool test_application::update()
{
    if(wnd->update() && !glfwGetKey(GLFW_KEY_ESC))
    {
        // Measure speed
        main_timer->tick();

        show_fps();

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        qsh->set();
        tex->set(0);
        q->draw();

        //glerr();


        wnd->swap_buffers();
        return true;
    }
    return false;
}

void test_application::show_fps()
{
    uint32_t currentTime = main_timer->get_time();
    frame_count++;
    if ( currentTime - last_time >= 1000 )  // If last prinf() was more than 1 sec ago
    {
        // printf and reset timer
        m_log->log(LOG_LOG,"FPS: %i (%f ms/frame)",frame_count,1000.0/double(frame_count));
        //printf("FPS: %i (%f ms/frame)\n",frame_count,1000.0/double(frame_count));
        frame_count = 0;
        last_time = currentTime;
    }
}

void test_application::exit()
{
    delete q;
    delete fbo_cache;
    delete tex_cache;
    delete shader_cache;
    delete mesh_cache;

    application::exit();
}
