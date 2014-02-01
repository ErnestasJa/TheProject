#include "precomp.h"
#include "sg_camera_object.h"

namespace sg
{

glm::vec3 cartesianToSpherical(glm::vec3 c)
{
    glm::vec3 ret(0,0,0);

    /// Radius
    ret.x=glm::length(c);

    /// Elevation
    ret.y=glm::asin(c.y/ret.x);

    /// Polar
    ret.z=glm::atan(c.x/c.z);
    if(c.z<0)
        ret.z+=glm::pi<float>();

    return ret;
}

glm::vec3 sphericalToCartesian(glm::vec3 s)
{
    glm::vec3 ret(0,0,0);

    float a=s.x * glm::cos(s.y);

    ret.x = a * glm::sin(s.z);
    ret.y = s.x * glm::sin(s.y);
    ret.z = a * glm::cos(s.z);

    return ret;
}

sg_camera_object::sg_camera_object(const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up, float aspect_ratio, float field_of_view, float near_z, float far_z)
{
    this->position=pos;
    this->target=target;
    this->up=up;
    m_projection = glm::perspective(field_of_view, aspect_ratio, near_z, far_z);

    glm::vec3 d=target-pos;
    glm::vec3 t=cartesianToSpherical(d);

    verticalAngle=t.y;
    horizontalAngle=t.z;

    speed=3.0;
    mouseSpeed=1.0;
}

sg_camera_object::~sg_camera_object()
{
}

uint32_t sg_camera_object::get_type()
{
    return SGO_CAMERA;
}

void sg_camera_object::mouse_moved(double delta_x, double delta_y)
{
    horizontalAngle+=mouseSpeed  * delta_x;
    verticalAngle+=mouseSpeed * delta_y;
    direction = glm::vec3(glm::cos(verticalAngle)*glm::sin(horizontalAngle),
                        glm::sin(verticalAngle),
                        glm::cos(verticalAngle)*glm::cos(horizontalAngle));

    right = glm::vec3(glm::sin(horizontalAngle - 3.14f/2.0f),
                    0,
                    glm::cos(horizontalAngle - 3.14f/2.0f));

    target=position+direction;
    up = glm::cross(right,direction);
    m_transform=glm::lookAt(position,target,up);
}

void sg_camera_object::key_pressed(int32_t key, int32_t scan_code, int32_t action, int32_t modifier)
{
    int kp=0;

    if(key == GLFW_KEY_W)
    {
        kp++;
        position+=direction*(float)(speed/kp);
    }
    if(key == GLFW_KEY_S)
    {
        kp++;
        position-=direction*(float)(speed/kp);
    }
    if(key == GLFW_KEY_A)
    {
        kp++;
        position-=right*(float)(speed/kp);
    }
    if(key == GLFW_KEY_D)
    {
        kp++;
        position+=right*(float)(speed/kp);
    }

    m_transform=glm::lookAt(position,target,up);
}

void sg_camera_object::render(scenegraph * sg){};
void sg_camera_object::on_set_shader_constants(shader_ptr shader){};

sg_material & sg_camera_object::get_material(uint32_t index){return m_mat;}
uint32_t sg_camera_object::get_material_count(){return 1;}

glm::mat4 & sg_camera_object::get_projection()
{
    return m_projection;
}

}
