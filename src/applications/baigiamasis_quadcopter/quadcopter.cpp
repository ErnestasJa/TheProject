#include "Precomp.h"
#include "scenegraph/Scenegraph.h"
#include "scenegraph/SGGraphicsManager.h"
#include "../../physics/physics.h"
#include "../AppContext.h"


#include "quadcopter.h"
using namespace sg;
quadcopter::quadcopter(app_context* app_ctx,int model,btVector3 init_pos,float init_height,btVector3 init_rotation,bool start_on)
{
    m_app_ctx=app_ctx;
    m_running=start_on;
    sg_graphics_manager_ptr gm=app_ctx->sg->get_graphics_manager();

    m_rotation=m_force=btVector3(0,0,0);
    m_transform=m_old_transform=btVector3(init_pos.x(),init_pos.y()+init_height,init_pos.z());
    m_mass=m_speed=m_power=m_delta=0;

    std::string quad_paths[4]= {"quadcopters/default_quad.iqm","quadcopters/unshielded_quad.iqm","quadcopters/micro_quad.iqm","quadcopters/fast_quad.iqm"};
    std::string quad_tex_paths[3]= {"quadcopters/red.png","quadcopters/green.png","quadcopters/blue.png"};
    std::string quad_sound_paths[2]= {"res/sounds/quad_sound.ogg","res/sounds/quad_sound_small.ogg"};

    m_height=init_height;

    switch(model)
    {
    case 0:
        m_mass=25;
        m_speed=10;
        m_power=1;

        setup[0]=0;
        setup[1]=1;
        setup[2]=0;
        break;
    case 1:
        m_mass=15;
        m_speed=15;
        m_power=1;

        setup[0]=1;
        setup[1]=0;
        setup[2]=0;
        break;
    case 2:
        m_mass=10;
        m_speed=12;
        m_power=1;

        setup[0]=2;
        setup[1]=2;
        setup[2]=1;
        break;
    case 3:
        m_mass=10;
        m_speed=12;
        m_power=1;

        setup[0]=3;
        setup[1]=2;
        setup[2]=1;
        break;
    default:
        break;
    }

    if(!make_a_quadcopter(quad_paths[setup[0]],quad_tex_paths[setup[1]],quad_sound_paths[setup[2]]))
        exit(-1);

}

bool quadcopter::make_a_quadcopter(std::string modelpath,std::string texpath,std::string soundpath)
{
    mesh_ptr m=m_app_ctx->gm->get_mesh_loader()->load(modelpath);
    if(!m) return false;

    sg::sg_mesh_object_ptr obj = sg::sg_mesh_object_ptr(new sg::sg_mesh_object(m_app_ctx->sg,m));

    sg::sg_material_static_mesh * sm_mat = static_cast<sg::sg_material_static_mesh*>(obj->get_material(0).get());
    sm_mat->mat_texture= m_app_ctx->gm->load_texture(texpath);

    m_app_ctx->sg->add_object(obj);

    m_body = m_app_ctx->pm->create_box(obj,25.0f);
    //m_app_ctx->sm->remove_sound("quad_idle");
    m_app_ctx->sm->add_sound(soundpath,"quad_idle",0.5,1);
    m_app_ctx->sm->play_sound_2d("quad_idle",true);

    return true;
}

quadcopter::~quadcopter()
{
    m_app_ctx->pm->destroy_body(m_body);
}

void quadcopter::variate_height(float step)
{
    m_height+=step*10*m_delta;
}

void quadcopter::variate_gyro(const glm::vec3 & gyro_step)
{
    glm::vec3 accelScaled=gyro_step/10.f;
    //if(accelScaled.x*10>0.25||accelScaled.x*10<-0.25)
    m_rotation.setX(-accelScaled.x);
    //if(accelScaled.y*10>0.25||accelScaled.y*10<-0.25)
    m_rotation.setZ(accelScaled.y);
}

void quadcopter::variate_axis(float rot)
{
    m_rotation.setY(m_rotation.y()+rot*5*m_delta);
}

float quadcopter::raycast_distance()
{
    return 0;
}

void quadcopter::update(float delta)
{
    m_delta=delta;

    btTransform tr=m_body->getCenterOfMassTransform();

    btQuaternion quat=tr.getRotation();

    quat.setEuler(m_rotation.y(),m_rotation.x(),m_rotation.z()); //or quat.setEulerZYX depending on the ordering you want
    tr.setRotation(quat);

    m_body->setAngularFactor(0);

    btVector3 b=m_rotation;
    //b=btVector3(b.z(),b.y(),b.x());
    b.safeNormalize();
    //force*=b;
    glm::vec3 dircalc(cos(m_rotation.x())*sin(m_rotation.y()),
                      sin(m_rotation.x()),
                      cos(m_rotation.x())*cos(m_rotation.y()));
    glm::vec3 right(sin(m_rotation.y()-3.14/2.f),0,cos(m_rotation.y()-3.14/2.f));



    m_transform=m_body->getCenterOfMassPosition();
    float transDelta=m_transform.y()-m_old_transform.y();

    if(m_transform.y()<m_height)
    {
        m_force= btVector3(0,-(9.83f)*25,0);
        //m_force*=btVector3(dircalc.x*-m_rotation.x(),1,dircalc.z*-m_rotation.x());
        //m_force+=btVector3(-right.x*25*9.83*m_rotation.z(),1,-right.z*25*9.83*m_rotation.z());
        float diff=m_transform.y()-m_height;
        m_force*=diff;
        transDelta*=25*9.83;
        transDelta*=diff;

        m_body->setLinearVelocity(btVector3((dircalc.x*m_rotation.x())-right.x*-m_rotation.z(),0,(dircalc.z*m_rotation.x())-right.z*-m_rotation.z())*m_speed);
        m_body->setFriction(0);
        m_body->applyImpulse((m_force+btVector3(0,transDelta,0))*m_delta,btVector3(-1,-1,1));
        m_body->applyImpulse((m_force+btVector3(0,transDelta,0))*m_delta,btVector3(1,-1,1));
        m_body->applyImpulse((m_force+btVector3(0,transDelta,0))*m_delta,btVector3(-1,-1,-1));
        m_body->applyImpulse((m_force+btVector3(0,transDelta,0))*m_delta,btVector3(1,-1,-1));
        m_old_transform=m_transform;
    }
    else
    {

    }
    m_body->setCenterOfMassTransform(tr);
    m_body->getMotionState()->setWorldTransform(tr);
}

void quadcopter::set_position(const glm::vec3 &v)
{
    btTransform trans=m_body->getCenterOfMassTransform();
    trans.setOrigin(physics_manager::glm_to_bt(v));
    m_body->setCenterOfMassTransform(trans);
    m_transform=m_body->getCenterOfMassPosition();
    m_old_transform=m_transform;
}

glm::vec3 quadcopter::get_position()
{
    return physics_manager::bt_to_glm(m_body->getCenterOfMassPosition());
}

void quadcopter::set_rotation(glm::vec3 rot)
{
    m_rotation=m_app_ctx->pm->glm_to_bt(rot);
    btTransform tr=m_body->getCenterOfMassTransform();

    btQuaternion quat;

    quat.setEuler(m_rotation.y(),m_rotation.x(),m_rotation.z());
    tr.setRotation(quat);
    m_body->setCenterOfMassTransform(tr);
}
