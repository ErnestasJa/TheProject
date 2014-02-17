#include "precomp.h"

#include "test_application.h"

#include "application/window.h"
#include "resources/resource_cache.h"
#include "opengl/texture.h"
#include "opengl/frame_buffer_object.h"
#include "utility/timer.h"
#include "utility/logger.h"
#include "opengl/quad.h"
#include "opengl/opengl_util.h"

#include "resources/iqmloader.h"
#include "resources/iqmesh.h"
#include "resources/image_loader.h"

test_application::test_application(uint32_t argc, const char ** argv): application(argc,argv)
{
    //definitely a comment
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

    img_loader = new image_loader();
    iqm_loader = new iqmloader(this->get_logger());

    frame_count = 0;
    last_time = 0;

    ///load our mesh
    char * buffer;

    if(!helpers::read("res/mrfixit.iqm",buffer))
        return false;

    iqmesh * mesh=iqm_loader->load(buffer);

    if(mesh)
    {
        mesh->generate();
        mesh_cache->push_back(share(mesh));
    }

    std::shared_ptr<image> img = share(img_loader->load("res/body.png"));
    texture * t = new texture();
    t->generate(img);

    if(!t)
        return false;

    ///quad shaders
    char * vsh=NULL;
    char * fsh=NULL;

    binding qtex_binding[]={{"tex",0},{"",-1}};

    if(!helpers::read("res/quad.vert",vsh)) return false;
    if(!helpers::read("res/quad.frag",fsh)) return false;

    qsh = new shader("quad_shader",vsh,fsh,qtex_binding,0);
	qsh->compile();
	qsh->link();
	shader_cache->push_back(share(qsh));

	delete [] vsh;
	delete [] fsh;

    ///mesh shaders
    if(!helpers::read("res/gpu_skin_normal.vert",vsh)) return false;
    if(!helpers::read("res/gpu_skin_normal.frag",fsh)) return false;

    sh = new shader("gpu_skin",vsh,fsh,qtex_binding,0);
	sh->compile();
	sh->link();
	shader_cache->push_back(share(sh));

	delete [] vsh;
	delete [] fsh;

	///prepare fbo, textures
	tex = new texture();
	ztex = new texture();

	auto shared_tex = share(tex);
	auto shared_ztex = share(ztex);

    tex->generate(NULL,GL_TEXTURE_2D,GL_RGBA,GL_RGBA,1024,1024);
    ztex->generate(NULL,GL_TEXTURE_2D,GL_DEPTH_COMPONENT,GL_DEPTH_COMPONENT24,1024,1024);

    tex_cache->push_back(shared_tex);
    tex_cache->push_back(shared_ztex);

    this->gl_util->check_and_output_errors();

    fbo = new frame_buffer_object();
    fbo->generate();
    fbo->enable_texture(0);
    fbo->set(FBO_WRITE);

    fbo->attach_texture(0,shared_tex);

    if(!fbo->is_complete())
        m_log->log(LOG_ERROR,"GL_FBO_ERROR: %s",gl_util->gl_fbo_error_to_string(fbo->get_status()).c_str());

    fbo->attach_depth_texture(shared_ztex);

    if(!fbo->is_complete())
        m_log->log(LOG_ERROR,"GL_FBO_ERROR: %s",gl_util->gl_fbo_error_to_string(fbo->get_status()).c_str());

    fbo_cache->push_back(share(fbo));

    q = new quad();
    q->generate();

    init_gl();

    ///render to fbo
    sh->set();
    glUniformMatrix4fv(sh->getparam("MVP"),1,GL_FALSE,glm::value_ptr(MVP));
    glUniformMatrix3x4fv(sh->getparam("bonemats"),mesh->data_header.num_joints,GL_FALSE,&mesh->current_frame[0][0].x);

    t->set(0);
    mesh->set_frame(80);
    mesh->draw(false);

    fbo->unset();

    int ww,hh;
    GLFWwindow * _window=wnd->getWindow();
    glfwGetWindowSize(_window,&ww,&hh);
    return true;
}

void test_application::init_gl()
{
    M = glm::mat4(1.0f);
    M = glm::translate(M,glm::vec3(-10,5,-10));
    M = glm::scale(M,glm::vec3(0.01,0.01,0.01));

    V = glm::lookAt(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0));
    V = glm::rotate<float>(V,-90,glm::vec3(0,1,0));

    P = glm::perspective(45.f,4.f/3.f,1.0f,2048.f);

    MVP=P*V*M;

    glClearColor(0.0f, 0.0f, 0.568f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}

bool test_application::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
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
        m_log->log(LOG_LOG,"FPS: %i (%f ms/frame)",frame_count,1000.0/double(frame_count));

        frame_count = 0;
        last_time = currentTime;
    }
}

void test_application::exit()
{
    delete img_loader;
    delete iqm_loader;
    delete q;
    delete fbo_cache;
    delete tex_cache;
    delete shader_cache;
    delete mesh_cache;

    application::exit();
}
