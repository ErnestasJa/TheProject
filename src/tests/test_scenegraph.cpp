#include "precomp.h"
#include "test_scenegraph.h"

#include "application/window.h"

#include "resources/resource_cache.h"
#include "opengl/texture.h"
#include "utility/timer.h"
#include "utility/logger.h"
#include "opengl/opengl_util.h"
#include "opengl/mesh.h"
#include "opengl/shader.h"

#include "resources/iqmloader.h"
#include "resources/image_loader.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/sg_mesh_object.h"

test_scenegraph::test_scenegraph(uint32_t argc, const char ** argv): application(argc,argv)
{

}

test_scenegraph::~test_scenegraph()
{
    //dtor
}


bool test_scenegraph::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);
    mesh_cache = create_resource_cache<mesh>();
    shader_cache = create_resource_cache<shader>();
    tex_cache = create_resource_cache<texture>();

    m_image_loader = new image_loader();
    m_iqm_loader = new iqmloader(this->get_logger());

    m_scenegraph = new sg::scenegraph();


    char * buf;

    if(!read("res/mrfixit.iqm",buf))
        return false;

    mesh_ptr m=m_iqm_loader->load(buf);

    if(!m)
        return false;

    m->generate();

    m_scenegraph->add_object(sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m)));

    char * vsh=NULL;
    char * fsh=NULL;

    binding qtex_binding[]={{"tex",0},{"",-1}};

    if(!read("res/static_mesh.vert",vsh)) return false;
    if(!read("res/static_mesh.frag",fsh)) return false;

    auto sh = share(new shader("gpu_skin",vsh,fsh,qtex_binding,0));
    m_shader = sh;
	sh->compile();
	sh->link();
	shader_cache->push_back(sh);

	delete [] vsh;
	delete [] fsh;

    init_gl();

    return !this->gl_util->check_and_output_errors();
}

void test_scenegraph::init_gl()
{
    M = glm::mat4(1.0f);

    M = glm::rotate<float>(M,-90,glm::vec3(1,0,0));

    V = glm::lookAt(glm::vec3(0,5,20),glm::vec3(0,5,0),glm::vec3(0,1,0));
    V = glm::rotate<float>(V,-90,glm::vec3(0,1,0));

    P = glm::perspective(45.f,4.f/3.f,1.0f,2048.f);

    MVP=P*V*M;

    glClearColor(0.0f, 0.0f, 0.568f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
}

bool test_scenegraph::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        main_timer->tick();

        cam_move();
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        if (auto spt = m_shader.lock())
        {
            spt->set();
            glUniformMatrix4fv(spt->getparam("MVP"),1,GL_FALSE,glm::value_ptr(MVP));

            m_scenegraph->render_all();
        }
        else
            return false;

        wnd->swap_buffers();

        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;
}

void test_scenegraph::cam_move()
{
    if(wnd->get_key(GLFW_KEY_W))
    {
        V = glm::translate(V,glm::vec3(-2,0,0));
    }
    else if(wnd->get_key(GLFW_KEY_S))
    {
        V = glm::translate(V,glm::vec3(2,0,0));
    }
    else if(wnd->get_key(GLFW_KEY_A))
    {
        V = glm::rotate<float>(V,-5,glm::vec3(0,1,0));
    }
    else if(wnd->get_key(GLFW_KEY_D))
    {
        V = glm::rotate<float>(V,5,glm::vec3(0,1,0));
    }

    MVP=P*V*M;
}


void test_scenegraph::exit()
{
    delete m_image_loader;
    delete m_iqm_loader;

    delete tex_cache;
    delete shader_cache;
    delete mesh_cache;

    application::exit();
}
