#pragma once

namespace sg
{
    class isg_object;
    typedef std::shared_ptr<isg_object> sg_object_ptr;
}

#include "../../scenegraph/sg_line_object.h"

class Objektas
{

public:
    static glm::vec3 calc_force(Objektas * obj1, Objektas * obj2, double grav_constant);

public:

    Objektas(sg::sg_object_ptr obj, sg::sg_line_object_ptr line_obj, float mass, glm::vec3 vel);

    void set_force(glm::vec3 force);
    glm::vec3 get_force();

    void set_mass(float mass);
    float get_mass();

    void set_velocity(glm::vec3 vel);
    glm::vec3 get_velocity();

    sg::sg_object_ptr get_object();
    sg::sg_line_object_ptr get_line_object();

protected:
    glm::vec3 m_force, m_velocity;
    float m_mass;
    sg::sg_object_ptr m_obj;
    sg::sg_line_object_ptr m_line_obj;
};

