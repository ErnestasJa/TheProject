#pragma once

namespace sg
{
    class isg_object;
    typedef std::shared_ptr<isg_object> sg_object_ptr;
}

#include "scenegraph/sg_line_object.h"

class Objektas
{

public:
    typedef glm::dvec3 (*accel_func)(Objektas *, Objektas *, double);

    static glm::dvec3 calc_accel(Objektas * obj1, Objektas * obj2, double grav_constant);
    static glm::dvec3 calc_accel_newton(Objektas * obj1, Objektas * obj2, double grav_constant);

    static glm::dvec3 calc_accel(Objektas * obj1, Objektas * obj2, double grav_constant, accel_func func);

    static bool check_collision(Objektas * obj1, Objektas * obj2);

public:

    Objektas(sg::sg_object_ptr obj, sg::sg_line_object_ptr line_obj, double mass, glm::dvec3 vel, glm::dvec3 pos);

    void set_position(glm::dvec3 position);
    glm::dvec3 get_position();

    void set_mass(double mass);
    double get_mass();

    void set_velocity(glm::dvec3 vel);
    glm::dvec3 get_velocity();

    void update_graphics_object();
    sg::sg_object_ptr get_object();
    sg::sg_line_object_ptr get_line_object();

protected:
    glm::dvec3 m_velocity, m_position;
    double m_mass;
    sg::sg_object_ptr m_obj;
    sg::sg_line_object_ptr m_line_obj;
};

