#include "test_application.h"

#include "iqmloader.h"
#include "window.h"
#include "timer.h"
#include "logger.h"
#include "quad.h"
#include "opengl_util.h"

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

    ///mesh shaders
    char * vsh=NULL;
    char * fsh=NULL;

    f = PHYSFS_openRead("res/gpu_skin_normal.vert");
    if(!f) return false;
    vsh = new char[PHYSFS_fileLength(f)+1];
    PHYSFS_read(f, vsh, 1, PHYSFS_fileLength(f));
    PHYSFS_close(f);


    f = PHYSFS_openRead("res/gpu_skin_normal.frag");
    if(!f) return false;
    fsh = new char[PHYSFS_fileLength(f)+1];
    PHYSFS_read(f, fsh, 1, PHYSFS_fileLength(f));
    PHYSFS_close(f);


    sh = new shader("gpu_skin",vsh,fsh,0,0);
	sh->compile();
	sh->link();
	shader_cache->push_back(share(sh));

	delete [] vsh;
	delete [] fsh;

	///quad shaders

    f = PHYSFS_openRead("res/quad.vert");
    if(!f) return false;
    vsh = new char[PHYSFS_fileLength(f)+1];
    PHYSFS_read(f, vsh, 1, PHYSFS_fileLength(f));
    PHYSFS_close(f);


    f = PHYSFS_openRead("res/quad.frag");
    if(!f) return false;
    fsh = new char[PHYSFS_fileLength(f)+1];
    PHYSFS_read(f, fsh, 1, PHYSFS_fileLength(f));
    PHYSFS_close(f);

    binding qtex_binding[]={{"tex",0},{"",-1}};
    qsh = new shader("quad_shader",vsh,fsh,qtex_binding,0);
	qsh->compile();
	qsh->link();
	shader_cache->push_back(share(qsh));

	delete [] vsh;
	delete [] fsh;

	///prepare fbo, textures

	tex = new texture();
	ztex = new texture();
	auto shared_tex = share(tex);
	auto shared_ztex = share(ztex);
    tex->generate(NULL,GL_TEXTURE_2D,GL_RGBA,GL_RGBA,1024,1024);
    ztex->generate(NULL,GL_TEXTURE_2D,GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT24,1024,1024,0);
    tex_cache->push_back(shared_tex);
    tex_cache->push_back(shared_ztex);
    this->gl_util->check_and_output_errors();

    fbo = new frame_buffer_object();
    fbo->generate();
    fbo->set(GL_FRAMEBUFFER);

    fbo->attach(GL_COLOR_ATTACHMENT0,shared_tex);
    uint32_t complete = fbo->is_complete();

    if(complete!=GL_FRAMEBUFFER_COMPLETE)
        m_log->log(LOG_ERROR,"GL_FBO_ERROR: %s",gl_util->gl_fbo_error_to_string(complete).c_str());

    fbo->attach(GL_DEPTH_ATTACHMENT,shared_ztex);
    complete = fbo->is_complete();

    if(complete!=GL_FRAMEBUFFER_COMPLETE)
        m_log->log(LOG_ERROR,"GL_FBO_ERROR: %s",gl_util->gl_fbo_error_to_string(complete).c_str());

    fbo_cache->push_back(share(fbo));

    q = new quad();
    q->generate();

    ///set up matrices

    M = glm::mat4(1.0f);
    M = glm::translate(M,glm::vec3(-10,5,-10));
    M = glm::scale(M,glm::vec3(0.01,0.01,0.01));

    V = glm::lookAt(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0));
    V = glm::rotate<float>(V,-90,glm::vec3(0,1,0));

    P = glm::perspective(45.f,4.f/3.f,1.0f,2048.f);

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

        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
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
