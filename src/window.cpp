#include "window.h"

std::map<GLFWwindow*, window*>   window::m_windows;
std::function< void(int32_t,int32_t) >  window::resize_callback;
