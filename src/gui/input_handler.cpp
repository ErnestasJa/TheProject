#include "precomp.h"

#include "math/vec2.h"
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

vec2<double> input_handler::get_mouse_pos()
{
    vec2<double> temp;
    glfwGetCursorPos(window,&temp.x,&temp.y);
    return temp;
}

bool input_handler::mouse_button(int btn)
{
    bool b=false;
    if(glfwGetMouseButton(window,btn)==1)
        b=true;
    return b;
}
