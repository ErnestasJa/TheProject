#include "precomp.h"
#include "test_game.h"

#include "application/window.h"

#include "resources/resource_cache.h"
#include "opengl/texture.h"
#include "opengl/render_buffer_object.h"
#include "opengl/frame_buffer_object.h"

#include "utility/timer.h"
#include "utility/logger.h"
#include "opengl/opengl_util.h"
#include "opengl/mesh.h"
#include "opengl/shader.h"


#include "resources/mesh_loader.h"
#include "resources/shader_loader.h"
#include "resources/image_loader.h"
#include "scenegraph/sg_graphics_manager.h"
#include "scenegraph/scenegraph.h"
#include "scenegraph/isg_render_queue.h"
#include "scenegraph/sg_scenegraph_loader.h"
#include "scenegraph/sg_objects.h"
#include "scenegraph/sg_material.h"
#include "physics/Physics.h"

//shadowmap texture dimensions
const int SHADOWMAP_DIMENSIONS = 1024
;

test_game::test_game(uint32_t argc, const char ** argv): application(argc,argv)
{
    //ctor
}

test_game::~test_game()
{
    //dtor
}

bool test_game::init(const std::string & title, uint32_t width, uint32_t height)
{
    application::init(title,width,height);
    wnd->sig_key_event().connect(sigc::mem_fun(this,&test_game::on_key_event));


    m_scenegraph = new sg::scenegraph(this->get_logger(),this->get_timer());
    m_graphics_manager = m_scenegraph->get_graphics_manager();

    m_physics_manager = new physics_manager(btVector3(0,-9.83f,0));

    if(!init_scene())
        return false;

    main_timer->tick();

    m_current_time = m_last_time = main_timer->get_real_time();

    return !this->gl_util->check_and_output_errors();
}

bool test_game::init_scene()
{
    ///hide cursor
    m_current_mouse_pos = m_last_mouse_pos = wnd->get_window_size()/2;
    m_log->log(LOG_LOG, "Window half size: [%i, %i]", m_current_mouse_pos.x, m_current_mouse_pos.y);

    wnd->set_cursor_disabled(true);
    ///for some reason this doulbe update is required in order not to have mouse pos jump at first mouse input events
    wnd->set_mouse_pos(m_current_mouse_pos);
    wnd->update();
    wnd->set_mouse_pos(m_current_mouse_pos);
    wnd->update();
    wnd->sig_mouse_moved().connect(sigc::mem_fun(this,&test_game::on_mouse_move));


    ///load scene
    //sg::sg_scenegraph_loader sg_loader;
    //sg_loader.load_scene(m_scenegraph,"res/maps/estate/estate.ibs");

    ///load map
    sg::sg_mesh_object_ptr obj=m_scenegraph->load_mesh_object("res/maps/nuke/nuke.iqm",true);

    m_scenegraph->add_object(obj);
    m_physics_manager->create_trimesh_body(obj,btVector3(1,1,1));

    sg::sg_camera_object_ptr cam = sg::sg_camera_object_ptr(new sg::sg_camera_object(m_scenegraph,glm::vec3(0,5,20),glm::vec3(0,0,0),glm::vec3(0,1,0)));

    m_scenegraph->set_active_camera(cam);

    sg::sg_light_object_ptr light = m_scenegraph->add_light_object();
    light->set_position(glm::vec3(0,20,0));

    glm::vec3 pos   = cam->get_position();
    glm::vec3 look  = glm::normalize(cam->get_look());

    ///load model
    mesh_ptr m=m_graphics_manager->get_mesh_loader()->load("res/quadcopter.iqm");
    if(!m) return false;

    obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_scenegraph,m));
    obj->set_position(glm::vec3(0,10,20));

    sg::sg_material_static_mesh * sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
    sm_mat->mat_texture= m_graphics_manager->load_texture("res/no_tex.png");

    m_scenegraph->add_object(obj);

    m_quadcopter = m_physics_manager->create_box(obj,25.0f);
    btVector3 abmin,abmax;
    m_quadcopter->getAabb(abmin,abmax);
    btVector3 ext1;
    //m_quadcopter->setLinearVelocity(m_physics_manager->glm_to_bt(look*1.0f));

    ///done loading

    if(this->gl_util->check_and_output_errors()) return false;

    ///shadow shit starts here
    //load all materials
    m_mat_first_pass = m_graphics_manager->create_material(sg::SGM_VSM_FIRST_PASS);
    m_mat_final_pass = m_graphics_manager->create_material(sg::SGM_VSM_FINAL_PASS);
    m_mat_gauss_v = m_graphics_manager->create_material(sg::SGM_TEXTURE_FILTER,"res/shaders/vsm/Passthrough.vert","res/shaders/vsm/GaussV.frag");
    m_mat_gauss_h = m_graphics_manager->create_material(sg::SGM_TEXTURE_FILTER,"res/shaders/vsm/Passthrough.vert","res/shaders/vsm/GaussH.frag");

    if(this->gl_util->check_and_output_errors())return false;

    //prepare fbos
    uint32_t w = wnd->get_window_size().x, h = wnd->get_window_size().y;
    glViewport(0,0,w,h);


    ///create textures
    texture * shadow_tex = new texture();
    texture_ptr shadow_tex_ptr(shadow_tex);
    shadow_tex->init(nullptr,GL_TEXTURE_2D,GL_RG,GL_RG32F,SHADOWMAP_DIMENSIONS,SHADOWMAP_DIMENSIONS);
    shadow_tex->set(0);
    shadow_tex->set_filters(texture::FILTER_MIN::FILTER_MIN_LINEAR_MIPMAP,texture::FILTER_MAG::FILTER_MAG_LINEAR);
    shadow_tex->set_clamp(texture::CLAMP::CLAMP_BORDER,texture::CLAMP::CLAMP_BORDER);
    shadow_tex->set_border_color(glm::vec4(1,1,0,0));
    shadow_tex->init_mipmap(0,4);
    shadow_tex->unset(0);
    m_shadow_tex = shadow_tex_ptr;

    texture * filter_tex = new texture();
    texture_ptr filter_tex_ptr(filter_tex);
    filter_tex->init(nullptr,GL_TEXTURE_2D,GL_RG,GL_RG32F,SHADOWMAP_DIMENSIONS,SHADOWMAP_DIMENSIONS);
    filter_tex->set(0);
    filter_tex->set_filters(texture::FILTER_MIN::FILTER_MIN_LINEAR,texture::FILTER_MAG::FILTER_MAG_LINEAR);
    filter_tex->set_clamp(texture::CLAMP::CLAMP_BORDER,texture::CLAMP::CLAMP_BORDER);
    filter_tex->set_border_color(glm::vec4(1,1,0,0));
    filter_tex->unset(0);

    render_buffer_object * rbo = new render_buffer_object();
    render_buffer_object_ptr rbo_ptr(rbo);
    rbo->init(GL_DEPTH_COMPONENT32,SHADOWMAP_DIMENSIONS,SHADOWMAP_DIMENSIONS);

    if(this->gl_util->check_and_output_errors())return false;

    ///prep fbos
    frame_buffer_object * fbo = new frame_buffer_object();
    frame_buffer_object_ptr fbo_ptr(fbo);
    m_shadow_fbo = fbo_ptr;
    fbo->init();
    fbo->set();

    fbo->attach_depth_texture(rbo_ptr);
    fbo->attach_texture(0,shadow_tex_ptr);

    if(!fbo->is_complete())
    {
        m_log->log(LOG_CRITICAL,"[FBO1] FUCK THIS SHIT, I QUIT!..");
        return false;
    }

    fbo->unset();
    if(this->gl_util->check_and_output_errors())return false;


    ///setup filtering fbo
    frame_buffer_object * fbo2 = new frame_buffer_object();
    frame_buffer_object_ptr fbo2_ptr(fbo2);
    fbo2->init();
    fbo2->set();

    fbo2->attach_texture(0,filter_tex_ptr);
    fbo2->attach_texture(1,shadow_tex_ptr);

    if(!fbo2->is_complete())
    {
        m_log->log(LOG_CRITICAL,"[FBO2] FUCK THIS SHIT, I QUIT!..");
        return false;
    }

    fbo2->unset();

    static_cast<sg::sg_material_texture_filter*>(m_mat_gauss_v.get())->texture0=shadow_tex_ptr;
    static_cast<sg::sg_material_texture_filter*>(m_mat_gauss_h.get())->texture0=filter_tex_ptr;
    static_cast<sg::sg_material_vsm_final_pass*>(m_mat_final_pass.get())->texture0=shadow_tex_ptr;

    if(this->gl_util->check_and_output_errors())return false;

    m_shadow_filter = fbo2_ptr;

    m_quad = share(new sg::sg_quad(m_scenegraph));

    ///enable depth testing and culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

    return true;
}

///only works if both are enabled, cause of fbo setup
bool blur = false;
bool physics = true;

bool test_game::update()
{
    if(wnd->update() && !wnd->get_key(GLFW_KEY_ESCAPE))
    {
        // Measure speed
        main_timer->tick();
        m_last_time = m_current_time;
        m_current_time = main_timer->get_real_time();
        float delta_time = ((float)(m_current_time - m_last_time)) / 1000.0f;

        if(physics)
            m_physics_manager->update(delta_time);

        m_scenegraph->update_all(delta_time);

        //m_scenegraph->validate_transforms();


        ///first pass
        glViewport(0, 0, SHADOWMAP_DIMENSIONS ,SHADOWMAP_DIMENSIONS);
        glClearColor(1,1,0,0);

        m_shadow_fbo->set();
        m_shadow_fbo->enable_buffer(0);

        m_scenegraph->set_override_material(m_mat_first_pass);

            m_scenegraph->render_all();

        m_scenegraph->set_override_material(nullptr);
        m_shadow_fbo->unset();

        ///filter first pass results
        //vertical blur
        glCullFace(GL_FRONT);
        m_shadow_filter->set();

        if(blur)
        {
            m_quad->set_material(m_mat_gauss_v);
            m_shadow_filter->enable_single_buffer(0);

            m_quad->register_for_rendering();
            m_scenegraph->get_render_queue()->render_all();

            m_quad->set_material(m_mat_gauss_h);
            m_shadow_filter->enable_single_buffer(1);

            m_quad->register_for_rendering();
            m_scenegraph->get_render_queue()->render_all();
        }

        m_shadow_filter->unset();
        glCullFace(GL_BACK);

        m_shadow_tex->update_mipmaps();

        ///draw scene "normally"
        uint32_t w = wnd->get_window_size().x, h = wnd->get_window_size().y;
        glViewport(0,0,w,h);
        glClearColor(0.2,0.8,0.2,0);
        m_scenegraph->set_override_material(m_mat_final_pass);

        m_scenegraph->render_all();

        m_scenegraph->set_override_material(nullptr);

        wnd->swap_buffers();

        trans=m_quadcopter->getCenterOfMassPosition();
        torq=btVector3(1,1,1)*m_quadcopter->getOrientation().getAxis()*m_quadcopter->getOrientation().getAngle();
        printf("TORQ %f %f %f\n",torq.x(),torq.y(),torq.y());
        if(trans.y()<10)
        {
            float transDelta=trans.y()-oldTrans.y();
            //printf("DELTA %f %f %f\n",torqDelta.x(),torqDelta.y(),torqDelta.y());
            btVector3 force(0,-(9.83f)*25,0);
            float diff=trans.y()-10;
            force*=diff;
            transDelta*=diff;
            transDelta*=100*25;

            //printf("TRANSDELTA: %f %f %f\n",transDelta.x(),transDelta.y(),transDelta.z());
            m_quadcopter->applyImpulse((force+btVector3(0,transDelta,0))*delta_time,btVector3(-1,-1,1));
            m_quadcopter->applyImpulse((force+btVector3(0,transDelta,0))*delta_time,btVector3(1,-1,1));
            m_quadcopter->applyImpulse((force+btVector3(0,transDelta,0))*delta_time,btVector3(-1,-1,-1));
            m_quadcopter->applyImpulse((force+btVector3(0,transDelta,0))*delta_time,btVector3(1,-1,-1));
            //m_quadcopter->applyTorqueImpulse((normalTorq+torqDelta)*100*25*9.83*delta_time);
            //m_quadcopter->applyImpulse(btVector3(0,2,0)*delta_time,trans+btVector3(-1,-1,0));
            //m_quadcopter->applyImpulse(btVector3(0,2,0)*delta_time,trans+btVector3(-1,-1,0));
            //m_quadcopter->applyImpulse(btVector3(0,2,0)*delta_time,btVector3(-1,-1,-1));
            oldTrans=trans;
            oldTorq=torq;
        }
        else
        {

        }
        ///let's just rage quit on gl error
        return !this->gl_util->check_and_output_errors();
    }
    return false;

}

void test_game::exit()
{
    m_graphics_manager = nullptr;

    delete m_scenegraph;
    delete m_physics_manager;

    application::exit();
}

bool test_game::init_physics()
{
    return true;
}

void test_game::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{
    if(action==GLFW_PRESS)
    {
        if(key==GLFW_KEY_1)
        {
            blur=!blur;
            m_log->log(LOG_LOG,"blur: %i", (int)blur);
        }

        if(key==GLFW_KEY_2)
        {
            physics=!physics;
            m_log->log(LOG_LOG,"physics: %i", (int)physics);
        }

    }

    if(sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera())
    {
        if(action==GLFW_PRESS || action == GLFW_REPEAT )
        {
            if(key==GLFW_KEY_W)
                cam->walk(1);
            if(key==GLFW_KEY_S)
                cam->walk(-1);
            if(key==GLFW_KEY_A)
                cam->strafe(-1);
            if(key==GLFW_KEY_D)
                cam->strafe(1);
        }

        if(action==GLFW_PRESS)
        {
            if(key==GLFW_KEY_SPACE)
            {
                glm::vec3 pos   = cam->get_position();
                glm::vec3 look  = glm::normalize(cam->get_look());

                ///load model
                mesh_ptr m=m_graphics_manager->get_mesh_loader()->load("res/trashcan.iqm");
                if(!m) return;

                sg::sg_mesh_object_ptr obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_scenegraph,m));
                obj->set_position(pos);

                sg::sg_material_static_mesh * sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
                sm_mat->mat_texture= m_graphics_manager->load_texture("res/no_tex.png");

                m_scenegraph->add_object(obj);

                btRigidBody * body = m_physics_manager->create_box(obj,10.0f);
                body->setLinearVelocity(m_physics_manager->glm_to_bt(look*10.0f));
            }

        }
    }
}

void test_game::on_mouse_move(double x, double y)
{
    m_last_mouse_pos = m_current_mouse_pos;
    m_current_mouse_pos = glm::ivec2(x,y);
    glm::ivec2 delta_pos =  m_current_mouse_pos - m_last_mouse_pos;

    if(glm::abs(delta_pos.x)>100||glm::abs(delta_pos.y)>100) ///probably some random shit happened, discard
        return;

    if(sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera())
    {
            glm::quat r = cam->get_rotation();
            glm::vec3 rot_deg = glm::eulerAngles(r);
            //m_log->log(LOG_LOG, "DELTA MOUSE [%i, %i]",delta_pos.x,delta_pos.y);
            //m_log->log(LOG_LOG, "Cam before rot[%f, %f, %f]",rot_deg.x,rot_deg.y,rot_deg.z);

            glm::quat rot_x(glm::vec3(0,-delta_pos.x/100.0f,0)), rot_y(glm::vec3(-delta_pos.y/100.0f,0,0));

            r= rot_x * r;
            r= r * rot_y;

            cam->set_rotation(r);

            r = cam->get_rotation();
            rot_deg = glm::eulerAngles(r);
            //m_log->log(LOG_LOG, "Cam after rot[%f, %f, %f]",rot_deg.x,rot_deg.y,rot_deg.z);
    }
}

void test_game::cam_move()
{

}
