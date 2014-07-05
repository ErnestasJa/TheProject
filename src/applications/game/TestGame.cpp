#include "Precomp.h"
#include "TestGame.h"

#include "Application/Window.h"

#include "resources/ResourceCache.h"
#include "opengl/Texture.h"
#include "opengl/RenderBufferObject.h"
#include "opengl/FrameBufferObject.h"

#include "utility/Timer.h"
#include "utility/Logger.h"
#include "opengl/OpenGLUtil.h"
#include "opengl/Mesh.h"
#include "opengl/Shader.h"


#include "resources/MeshLoader.h"
#include "resources/ShaderLoader.h"
#include "resources/ImageLoader.h"
#include "scenegraph/SGGraphicsManager.h"
#include "scenegraph/SGScenegraph.h"
#include "scenegraph/ISGRenderQueue.h"
#include "scenegraph/SGScenegraphLoader.h"
#include "scenegraph/SGObjects.h"
#include "scenegraph/SGMaterial.h"
#include "physics/Physics.h"

#include "utility/Rect2d.h"
#include "gui/GUISkin.h"
#include "gui/GUIEnvironment.h"
#include "gui/GUIButton.h"
#include "gui/GUIStaticText.h"
#include "gui/GUICheckbox.h"
#include "gui/GUIPane.h"
#include "gui/GUIEditBox.h"
#include "gui/GUIImage.h"
#include "gui/GUIWindow.h"
#include "gui/GUISlider.h"

#include "network/NetworkManagerWin32.h"

#include "application/AppContext.h"

#define NETWORKINGs

/// //////////////////////////////////////////// ///
/// WARNING: IMPLEMENT APP CONTEXT BEFORE USING! ///
/// //////////////////////////////////////////// ///

//shadowmap texture dimensions
const int SHADOWMAP_DIMENSIONS = 1024;

test_game::test_game(uint32_t argc, const char ** argv): Application(argc,argv)
{
    //ctor
    _appContext=new AppContext();
}

test_game::~test_game()
{
    //dtor
    #ifdef NETWORKING
    delete m_netwman;
    #endif // NETWORKING
    delete env;
}

bool test_game::Init(const std::string & title, uint32_t width, uint32_t height)
{
    Application::Init(title,width,height);
    _window->SigKeyEvent().connect(sigc::mem_fun(this,&test_game::on_key_event));


    m_scenegraph = new sg::scenegraph(this->GetLogger(),this->GetTimer());
    m_graphics_manager = m_scenegraph->get_graphics_manager();

    m_physics_manager = new physics_manager(btVector3(0,-9.83f,0));

    if(!init_scene())
        return false;

    init_gui();

    m_cam_fps=false;

    m_netwman=new network_manager_win32();

    #ifdef NETWORKING
    if(!m_netwman->init())
    {
        printf("NETWMAN: Init failed.\n");
        return false;
    }
    #endif // NETWORKING

    //m_netwman->start_thread();

    _mainTimer->tick();

    m_current_time = m_last_time = _mainTimer->get_real_time();

    return !this->_GLUtil->check_and_output_errors();
}

bool test_game::init_gui()
{
    gui_skin s=gui_skin();
    s.load("../../res/skin_default.xml");

    env=new gui_environment(this->_window,this->GetLogger());

    gui_window * wind  = new gui_window(env,rect2d<int>(10,10,256,512),L"ąčęėįšūž");

    #define init_e(x) x->set_parent(wind); x->set_event_listener(this)

    gui_static_text * tb = new gui_static_text(env,rect2d<int>(10,30,48,20),L"Height:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);

    gui_slider* slider = new gui_slider(env,rect2d<int>(96,30,64,20),0,20,20);
    slider->set_name("quad_height_slider");
    init_e(slider);
    m_quad_height=slider->get_value();

    tb = new gui_static_text(env,rect2d<int>(10,50,48,20),L"XRot:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);

    slider = new gui_slider(env,rect2d<int>(96,50,64,20),-3.14/180*30,3.14/180*30,0);
    slider->set_name("quad_rot_x_slider");
    init_e(slider);
    trgRot.setX(slider->get_value());

    tb = new gui_static_text(env,rect2d<int>(10,70,48,20),L"YRot:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);

    slider = new gui_slider(env,rect2d<int>(96,70,64,20),-3.14,3.14,0);
    slider->set_name("quad_rot_y_slider");
    init_e(slider);
    trgRot.setY(slider->get_value());

    tb = new gui_static_text(env,rect2d<int>(10,90,48,20),L"ZRot:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);

    slider = new gui_slider(env,rect2d<int>(96,90,64,20),-3.14/180*30,3.14/180*30,0);
    slider->set_name("quad_rot_z_slider");
    init_e(slider);
    trgRot.setZ(slider->get_value());

    tb = new gui_static_text(env,rect2d<int>(10,110,48,20),L"Direction:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);
    quad_dir = new gui_static_text(env,rect2d<int>(60,110,48,20),L"",glm::vec4(0,0,0,1),false,false);
    init_e(quad_dir);

    tb = new gui_static_text(env,rect2d<int>(10,130,48,20),L"Height:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);
    quad_height = new gui_static_text(env,rect2d<int>(60,130,48,20),L"",glm::vec4(0,0,0,1),false,false);
    init_e(quad_height);

    tb = new gui_static_text(env,rect2d<int>(10,150,48,20),L"Torque:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);
    quad_torq = new gui_static_text(env,rect2d<int>(60,150,48,20),L"",glm::vec4(0,0,0,1),false,false);
    init_e(quad_torq);

    tb = new gui_static_text(env,rect2d<int>(10,170,48,20),L"Calc:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);
    quad_torqd = new gui_static_text(env,rect2d<int>(60,170,48,20),L"",glm::vec4(0,0,0,1),false,false);
    init_e(quad_torqd);

    tb = new gui_static_text(env,rect2d<int>(10,190,48,20),L"Force:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);
    quad_force = new gui_static_text(env,rect2d<int>(60,190,48,20),L"",glm::vec4(0,0,0,1),false,false);
    init_e(quad_force);

    tb = new gui_static_text(env,rect2d<int>(10,210,48,20),L"DeltaForce:",glm::vec4(0,0,0,1),false,false);
    init_e(tb);
    quad_forced = new gui_static_text(env,rect2d<int>(60,210,48,20),L"",glm::vec4(0,0,0,1),false,false);
    init_e(quad_forced);

    gui_edit_box *eb=new gui_edit_box(env,rect2d<int>(10,230,200,200),L"",glm::vec4(0,0,0,1),false,false,false);
    init_e(eb);

    gui_button *btn=new gui_button(env,rect2d<int>(10,450,200,20),L"Yep, a button");
    init_e(btn);

    env->set_event_listener(this);

    return true;
}

bool test_game::init_scene()
{
    ///hide cursor
    m_current_mouse_pos = m_last_mouse_pos = _window->GetWindowSize()/2;
    _logger->log(LOG_LOG, "Window half size: [%i, %i]", m_current_mouse_pos.x, m_current_mouse_pos.y);

    //wnd->set_cursor_disabled(true);
    ///for some reason this doulbe update is required in order not to have mouse pos jump at first mouse input events
    _window->set_mouse_pos(m_current_mouse_pos);
    _window->Update();
    _window->set_mouse_pos(m_current_mouse_pos);
    _window->Update();
    _window->SigMouseMoved().connect(sigc::mem_fun(this,&test_game::on_mouse_move));


    ///load scene
    //sg::sg_scenegraph_loader sg_loader;
    //sg_loader.load_scene(m_scenegraph,"res/maps/estate/estate.ibs");

    ///load map
    sg::sg_mesh_object_ptr obj=m_scenegraph->load_mesh_object("res/maps/aztec/aztec_map.iqm",true);

    m_scenegraph->add_object(obj);
    m_physics_manager->create_trimesh_body(obj,btVector3(1,1,1));

    sg::sg_camera_object_ptr cam = sg::sg_camera_object_ptr(new sg::sg_camera_object(_appContext,glm::vec3(0,5,20),glm::vec3(0,0,0),glm::vec3(0,1,0)));

    m_scenegraph->set_active_camera(cam);

    sg::sg_light_object_ptr light = m_scenegraph->add_light_object();
    light->set_position(glm::vec3(0,100,0));

    glm::vec3 pos   = cam->get_position();
    glm::vec3 look  = glm::normalize(cam->get_look());

    ///load model
    mesh_ptr m=m_graphics_manager->get_mesh_loader()->load("res/quadcopter.iqm");
    if(!m) return false;

    obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_scenegraph,m));
    obj->set_position(glm::vec3(0,20,0));

    sg::sg_material_static_mesh * sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
    sm_mat->mat_texture= m_graphics_manager->load_texture("res/no_tex.png");

    m_scenegraph->add_object(obj);

    m_quadcopter = m_physics_manager->create_box(obj,25.0f);
    btVector3 abmin,abmax;
    m_quadcopter->getAabb(abmin,abmax);
    btVector3 ext1;
    //m_quadcopter->setLinearVelocity(m_physics_manager->glm_to_bt(look*1.0f));

    ///done loading

    if(this->_GLUtil->check_and_output_errors()) return false;

    ///shadow shit starts here
    //load all materials
    m_mat_first_pass = m_graphics_manager->create_material(sg::SGM_VSM_FIRST_PASS);
    m_mat_final_pass = m_graphics_manager->create_material(sg::SGM_VSM_FINAL_PASS);
    m_mat_gauss_v = m_graphics_manager->create_material(sg::SGM_TEXTURE_FILTER,"res/shaders/vsm/Passthrough.vert","res/shaders/vsm/GaussV.frag");
    m_mat_gauss_h = m_graphics_manager->create_material(sg::SGM_TEXTURE_FILTER,"res/shaders/vsm/Passthrough.vert","res/shaders/vsm/GaussH.frag");

    if(this->_GLUtil->check_and_output_errors())return false;

    //prepare fbos
    uint32_t w = _window->GetWindowSize().x, h = _window->GetWindowSize().y;
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

    if(this->_GLUtil->check_and_output_errors())return false;

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
        _logger->log(LOG_CRITICAL,"[FBO1] FUCK THIS SHIT, I QUIT!..");
        return false;
    }

    fbo->unset();
    if(this->_GLUtil->check_and_output_errors())return false;


    ///setup filtering fbo
    frame_buffer_object * fbo2 = new frame_buffer_object();
    frame_buffer_object_ptr fbo2_ptr(fbo2);
    fbo2->init();
    fbo2->set();

    fbo2->attach_texture(0,filter_tex_ptr);
    fbo2->attach_texture(1,shadow_tex_ptr);

    if(!fbo2->is_complete())
    {
        _logger->log(LOG_CRITICAL,"[FBO2] FUCK THIS SHIT, I QUIT!..");
        return false;
    }

    fbo2->unset();

    static_cast<sg::sg_material_texture_filter*>(m_mat_gauss_v.get())->texture0=shadow_tex_ptr;
    static_cast<sg::sg_material_texture_filter*>(m_mat_gauss_h.get())->texture0=filter_tex_ptr;
    static_cast<sg::sg_material_vsm_final_pass*>(m_mat_final_pass.get())->texture0=shadow_tex_ptr;

    if(this->_GLUtil->check_and_output_errors())return false;

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

bool test_game::Update()
{
    #ifdef NETWORKING
    if(!m_netwman->is_receiving())
        return false;
    #endif // NETWORKING
    if(_window->Update() && !_window->GetKey(GLFW_KEY_ESCAPE))
    {
        //if(!m_netwman->update()){printf("Could not update..\n");return false;}
        // Measure speed
        _mainTimer->tick();
        m_last_time = m_current_time;
        m_current_time = _mainTimer->get_real_time();
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
        uint32_t w = _window->GetWindowSize().x, h = _window->GetWindowSize().y;
        glViewport(0,0,w,h);
        glClearColor(0.6,0.2,0.2,0);
        m_scenegraph->set_override_material(m_mat_static_mesh);

        m_scenegraph->render_all();

        m_scenegraph->set_override_material(nullptr);

        //glDisable(GL_CULL_FACE);
        env->update(0);
        env->render();
        //glEnable(GL_CULL_FACE);

        _window->swap_buffers();

        ///QUADCOPTER STUFF
        glm::vec3 accel=m_netwman->get_accelerometer_data();
        glm::vec3 accelScaled=accel/10.f;
        //if(accelScaled.x*10>0.25||accelScaled.x*10<-0.25)
            trgRot.setX(-accelScaled.x);
        //if(accelScaled.y*10>0.25||accelScaled.y*10<-0.25)
            trgRot.setZ(accelScaled.y);
        trgRot.setY(trgRot.y()+(float)(m_netwman->get_rot_diff()*2*delta_time));

        rot=btVector3(0,0,0);
        rot.setX(helpers::coslerp(oldRot.x(),trgRot.x(),0.5f));
        rot.setX(helpers::coslerp(oldRot.y(),trgRot.y(),0.5f));
        rot.setX(helpers::coslerp(oldRot.z(),trgRot.z(),0.5f));

        btTransform tr=m_quadcopter->getCenterOfMassTransform();
        btQuaternion quat=tr.getRotation();
        quat.setEuler(trgRot.y(),trgRot.x()+rot.x()/10,trgRot.z()+rot.z()/10); //or quat.setEulerZYX depending on the ordering you want
        tr.setRotation(quat);

        m_quadcopter->setAngularFactor(0);

        btVector3 b=btVector3(trgRot.x(),trgRot.y(),trgRot.z());
        //b=btVector3(b.z(),b.y(),b.x());
        b.safeNormalize();
        //force*=b;
        std::wstringstream ss;
        ss<<b.x()<<L","<<b.y()<<L","<<b.z()<<L";";
        quad_dir->set_text(ss.str().c_str());

        ss.str(L"");
        ss<<trgRot.x()<<L","<<trgRot.y()<<L","<<trgRot.z()<<L";";
        quad_torq->set_text(ss.str().c_str());

        glm::vec3 dircalc(cos(trgRot.x())*sin(trgRot.y()),
        sin(trgRot.x()),
        cos(trgRot.x())*cos(trgRot.y()));
        glm::vec3 right(sin(trgRot.y()-3.14/2.f),0,cos(trgRot.y()-3.14/2.f));

        ss.str(L"");
        ss<<dircalc.x<<L","<<dircalc.y<<L","<<dircalc.z<<L";";
        quad_torqd->set_text(ss.str().c_str());

        ss.str(L"");
        ss<<right.x<<L","<<right.y<<L","<<right.z<<L";";
        quad_force->set_text(ss.str().c_str());


        m_quad_height+=m_netwman->get_height_diff()*5*delta_time;
        trans=m_quadcopter->getCenterOfMassPosition();
        float transDelta=trans.y()-oldTrans.y();
        if(trans.y()<m_quad_height)
        {
            //m_quadcopter->setLinearVelocity(btVector3(dircalc.x,dircalc.y,dircalc.z));
            btVector3 force(25*9.83,-(9.83f)*25,25*9.83);
            force*=btVector3(dircalc.x*-trgRot.x(),1,dircalc.z*-trgRot.x());
            force+=btVector3(-right.x*25*9.83*trgRot.z(),1,-right.z*25*9.83*trgRot.z());
            float diff=trans.y()-m_quad_height;
            force*=diff;
            transDelta*=25*9.83*25;
            transDelta*=diff;

            m_quadcopter->applyImpulse((force+btVector3(0,transDelta,0))*delta_time,btVector3(-1,-1,1));
            m_quadcopter->applyImpulse((force+btVector3(0,transDelta,0))*delta_time,btVector3(1,-1,1));
            m_quadcopter->applyImpulse((force+btVector3(0,transDelta,0))*delta_time,btVector3(-1,-1,-1));
            m_quadcopter->applyImpulse((force+btVector3(0,transDelta,0))*delta_time,btVector3(1,-1,-1));
            oldTrans=trans;
        }
        else
        {

        }
        m_quadcopter->setCenterOfMassTransform(tr);
        m_quadcopter->getMotionState()->setWorldTransform(tr);
        oldRot=trgRot;
        sg::sg_camera_object_ptr cam = m_scenegraph->get_active_camera();
        if(!m_cam_fps)
        cam->orbit(physics_manager::bt_to_glm(trans),5,80,180+trgRot.y()*180.f/glm::pi<float>());

        ///let's just rage quit on gl error
        return !this->_GLUtil->check_and_output_errors();
    }
    return false;

}

void test_game::Exit()
{
    m_graphics_manager = nullptr;

    delete m_scenegraph;
    delete m_physics_manager;

    Application::Exit();
}

bool test_game::init_physics()
{
    return true;
}

bool test_game::on_event(const gui_event & e)
{
    switch(e.get_type())
    {
        case button_pressed:
        {
            _logger->log(LOG_LOG, "Got event type: %i\nElement name: '%s'.", e.get_type(), e.get_caller()->get_name().c_str());
            break;
        }
        case scrollbar_changed:
        {
            if(e.get_caller()->get_name()=="quad_height_slider")
            {
                gui_slider * s = static_cast<gui_slider *>(e.get_caller());
                m_quad_height = s->get_value();
            }
            if(e.get_caller()->get_name()=="quad_rot_x_slider")
            {
                gui_slider * s = static_cast<gui_slider *>(e.get_caller());
                trgRot.setX(s->get_value());
            }
            if(e.get_caller()->get_name()=="quad_rot_y_slider")
            {
                gui_slider * s = static_cast<gui_slider *>(e.get_caller());
                trgRot.setY(s->get_value());
            }
            if(e.get_caller()->get_name()=="quad_rot_z_slider")
            {
                gui_slider * s = static_cast<gui_slider *>(e.get_caller());
                trgRot.setZ(s->get_value());
            }

            break;
        }
        default:
            break;
    }
    return false;
}

void test_game::on_key_event(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{
    if(action==GLFW_PRESS)
    {
        if(key==GLFW_KEY_1)
        {
            blur=!blur;
            _logger->log(LOG_LOG,"blur: %i", (int)blur);
        }

        if(key==GLFW_KEY_2)
        {
            physics=!physics;
            _logger->log(LOG_LOG,"physics: %i", (int)physics);
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

        if(action==GLFW_RELEASE)
        {
            if(key==GLFW_KEY_F)
            {
                m_cam_fps=!m_cam_fps;
            }
        }
    }
}

void test_game::on_mouse_move(double x, double y)
{
    if(m_cam_fps)
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
            //_logger->log(LOG_LOG, "DELTA MOUSE [%i, %i]",delta_pos.x,delta_pos.y);
            //_logger->log(LOG_LOG, "Cam before rot[%f, %f, %f]",rot_deg.x,rot_deg.y,rot_deg.z);

            glm::quat rot_x(glm::vec3(0,-delta_pos.x/100.0f,0)), rot_y(glm::vec3(-delta_pos.y/100.0f,0,0));

            r= rot_x * r;
            r= r * rot_y;

            cam->set_rotation(r);

            r = cam->get_rotation();
            rot_deg = glm::eulerAngles(r);
            //_logger->log(LOG_LOG, "Cam after rot[%f, %f, %f]",rot_deg.x,rot_deg.y,rot_deg.z);
    }

    }
}

void test_game::cam_move()
{

}
