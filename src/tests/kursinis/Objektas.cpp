#include "precomp.h"
#include "Objektas.h"
#include "scenegraph/isg_object.h"

glm::dvec3 Objektas::calc_accel(Objektas * obj1, Objektas * obj2, double grav_constant)
{
    glm::dvec3 p = obj1->get_position(), p_m = obj2->get_position();
    double m  = obj2->get_mass();

    glm::dvec3 r = p-p_m;
    double r_mag = glm::length(r);
    glm::dvec3 r_norm = glm::normalize(r);
    glm::dvec3 a = (-(double)grav_constant*m/(double)pow(r_mag,2.0f)) * r_norm;
    return a;
}

glm::dvec3 Objektas::calc_accel_newton(Objektas * obj1, Objektas * obj2, double grav_constant)
{
    glm::dvec3  p1 = obj1->get_position(),
                p2 = obj2->get_position();
    double      m  = obj2->get_mass();
    glm::dvec3  r = p1-p2;
    double      r_mag = glm::length(r);
    glm::dvec3  r_norm = glm::normalize(r);
    double      G = grav_constant;

    glm::dvec3 a = (-(double)G*m/(double)pow(r_mag,3.0f)) * r;
    return a;
}

glm::dvec3 Objektas::calc_accel(Objektas * obj1, Objektas * obj2, double grav_constant, accel_func func)
{
    return func(obj1,obj2,grav_constant);
}

bool Objektas::check_collision(Objektas * obj1, Objektas * obj2)
{
    return glm::distance(obj1->get_object()->get_position(),obj2->get_object()->get_position())<=2.0f;
}

Objektas::Objektas(sg::sg_object_ptr obj, sg::sg_line_object_ptr line_obj, double mass, glm::dvec3 vel, glm::dvec3 pos)
{
    m_obj = obj;
    m_line_obj = line_obj;
    m_mass = mass;
    m_velocity = vel;
    m_position = pos;
}

void Objektas::set_velocity(glm::dvec3 vel)
{
    m_velocity = vel;
}

glm::dvec3 Objektas::get_velocity()
{
    return m_velocity;
}

void Objektas::set_position(glm::dvec3 pos)
{
    m_position = pos;
}

glm::dvec3 Objektas::get_position()
{
    return m_position;
}

void Objektas::set_mass(double mass)
{
    m_mass = mass;
}

double Objektas::get_mass()
{
    return m_mass;
}

void Objektas::update_graphics_object()
{
    this->m_obj->set_position(glm::vec3(m_position));
}

sg::sg_object_ptr Objektas::get_object()
{
    return m_obj;
}

sg::sg_line_object_ptr Objektas::get_line_object()
{
    return m_line_obj;
}
