#include "Precomp.h"
#include "utility/Logger.h"
#include "Camera.h"
#include "application/AppContext.h"

Camera::Camera(AppContext* ctx, const glm::vec3 &pos,const glm::vec3 &target,const glm::vec3 &up, float aspect_ratio, float field_of_view, float near_z, float far_z)
{
    _appContext=ctx;
    m_fps=true;
    ///this->set_position(pos);
    m_pos=pos;
    ///this->set_rotation(glm::toQuat(glm::inverse(glm::lookAt(pos,target,up))));
    m_rot=glm::toQuat(glm::inverse(glm::lookAt(pos,target,up)));

    this->m_look=pos-target;
    this->m_up=up;

    m_fov = field_of_view;
    m_far = far_z;
    m_near = near_z;
    m_aspect_ratio = aspect_ratio;

    m_P = glm::perspective(field_of_view, aspect_ratio, near_z, far_z);

    ///this->update_absolute_transform();
}

Camera::~Camera()
{
}

glm::mat4 & Camera::GetProjectionMat()
{
    return m_P;
}

const float Camera::GetFOV() const
{
    return m_fov;
}

const float Camera::GetFar() const
{
    return m_far;
}

const float Camera::GetNear() const
{
    return m_near;
}

const float Camera::GetAspectRatio() const
{
    return m_aspect_ratio;
}

const glm::vec3 Camera::GetLook() const
{
    return m_look;
}

void Camera::Update(float dt)
{
    m_pos+=m_translation;
    m_translation=glm::vec3(0);

    if(m_fps)
    {
        glm::ivec2 s=_appContext->_window->GetWindowSize();
        m_current_mouse_pos=_appContext->_window->GetMousePos();
        _appContext->_window->set_mouse_pos(s/2);
        m_last_mouse_pos=_appContext->_window->GetMousePos();
        HandleMouse();
    }
}

void Camera::Walk(const float amount)
{
    m_translation += (m_look*amount);
}

void Camera::Strafe(const float amount)
{
    m_translation += (m_right*amount);
}

void Camera::Lift(const float amount)
{
    m_translation += (m_up*amount);
}

glm::mat4x4 Camera::GetViewMat()
{
    m_look  = glm::vec3(m_rot*glm::vec3(0,0,-1));
    m_up    = glm::vec3(m_rot*glm::vec3(0,1,0));

    m_right = glm::cross(m_look, m_up);

    return glm::lookAt(m_pos,m_pos+m_look,m_up);
}

glm::mat4x4 Camera::GetViewProjMat()
{
    return GetProjectionMat()*GetViewMat();
}

void Camera::Orbit(glm::vec3 point,float distance,float angleX,float angleY)
{
    float r,phi,theta;
    r=distance;
    phi=angleX;
    theta=angleY;

    float camX = r * sin(glm::radians(theta)) * sin(glm::radians(phi));
    float camY = r * cos(glm::radians(phi));
    float camZ = r * cos(glm::radians(theta)) * sin(glm::radians(phi));
    glm::vec3 camoffs(camX,camY,camZ);
    glm::vec3 camPos=point+camoffs;

    m_pos=camPos;
    m_rot=glm::toQuat(glm::inverse(glm::lookAt(camPos,point,glm::vec3(0,1,0))));
    ///this->update_absolute_transform();
}

void Camera::HandleMouse()
{
        glm::ivec2 delta_pos =  m_current_mouse_pos - m_last_mouse_pos;

        if(glm::abs(delta_pos.x)>100||glm::abs(delta_pos.y)>100) ///probably some random shit happened, discard
            return;

        glm::quat r = m_rot;
        glm::vec3 rot_deg = glm::eulerAngles(r);

        glm::quat rot_x(glm::vec3(0,-delta_pos.x/100.0f,0)), rot_y(glm::vec3(-delta_pos.y/100.0f,0,0));

        r= rot_x * r;
        r= r * rot_y;

        m_rot=r;
}