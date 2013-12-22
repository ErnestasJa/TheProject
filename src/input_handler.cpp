#include "precomp.h"
#include "vec2.h"
#include "logger.h"
#include "input_handler.h"

input_handler::input_handler(logger *log)
{
    //ctor
    //this->log=log;
    //this->log->log(LOG_DEBUG,"Input handler created.");
}

input_handler::~input_handler()
{
    //dtor
}

vec2<int> input_handler::get_mouse_pos()
{
    vec2<int> temp;
    glfwGetMousePos(&temp.x,&temp.y);
    return temp;
}

bool input_handler::mouse_button(int btn)
{
    bool b=false;
    if(glfwGetMouseButton(btn)==1)
        b=true;
    return b;
}
