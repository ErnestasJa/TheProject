#include "precomp.h"

#include "window.h"

std::map<GLFWwindow*, window*>   window::m_windows;

void mouse_move(GLFWwindow * wnd, double x, double y)
{
    window::m_windows[wnd]->sig_mouse_moved().emit(x,y);
}

void window_resize(GLFWwindow * wnd, int32_t w, int32_t h)
{
    window::m_windows[wnd]->sig_window_resized().emit(w,h);
}

void window_close(GLFWwindow * wnd)
{
    window::m_windows[wnd]->sig_window_closed().emit(window::m_windows[wnd]);
    window::m_windows[wnd] = nullptr;
}

void key_event(GLFWwindow * wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
    window::m_windows[wnd]->sig_key_event().emit(key,scan_code,action,modifiers);
}
