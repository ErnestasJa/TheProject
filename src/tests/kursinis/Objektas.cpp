#include "precomp.h"
#include "Objektas.h"
#include "scenegraph/isg_object.h"

///gravitacine konstanta
static const float G = 6.67/1000.0;

glm::vec3 Objektas::calc_force(Objektas * obj1, Objektas * obj2)
{
    glm::vec3 p = obj1->get_object()->get_position(), p_m = obj2->get_object()->get_position();
    float m  = obj2->get_mass();

    glm::vec3 r = p-p_m;
    float r_mag = glm::length(r);
    glm::vec3 r_norm = glm::normalize(r);
    glm::vec3 a = (-G*m/(float)pow(r_mag,2.0f)) * r_norm;
    return a;
}

Objektas::Objektas(sg::sg_object_ptr obj, sg::sg_line_object_ptr line_obj, float mass, glm::vec3 vel)
{
    m_obj = obj;
    m_line_obj = line_obj;
    m_mass = mass;
    m_velocity = vel;
}

void Objektas::set_force(glm::vec3 force)
{
    m_force = force;
}

glm::vec3 Objektas::get_force()
{
    return m_force;
}

void Objektas::set_velocity(glm::vec3 vel)
{
    m_velocity = vel;
}

glm::vec3 Objektas::get_velocity()
{
    return m_velocity;
}

void Objektas::set_mass(float mass)
{
    m_mass = mass;
}

float Objektas::get_mass()
{
    return m_mass;
}

sg::sg_object_ptr Objektas::get_object()
{
    return m_obj;
}

sg::sg_line_object_ptr Objektas::get_line_object()
{
    return m_line_obj;
}
