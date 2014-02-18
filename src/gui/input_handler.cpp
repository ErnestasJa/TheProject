#include "precomp.h"

#include "utility/logger.h"
#include "gui/input_handler.h"

input_handler::input_handler(logger *log,GLFWwindow* win)
{
    //ctor
    //this->log=log;
    //this->log->log(LOG_DEBUG,"Input handler created.");

    window=win;
}

input_handler::~input_handler()
{
    //dtor
}

glm::vec2 input_handler::get_mouse_pos()
{
    glm::dvec2 temp;
    glfwGetCursorPos(window,&temp.x,&temp.y);
    glm::vec2 ret=glm::vec2(temp);
    return ret;
}

bool input_handler::mouse_button(int btn)
{
    bool b=false;
    if(glfwGetMouseButton(window,btn)==1)
        b=true;
    return b;
}