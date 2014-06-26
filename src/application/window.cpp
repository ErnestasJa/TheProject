#include "precomp.h"

#include "window.h"

std::map<GLFWwindow*, window*>   window::m_windows;

void mouse_move_event(GLFWwindow * wnd, double x, double y)
{
    window::m_windows[wnd]->sig_mouse_moved().emit(x,y);
}

void mouse_key_event(GLFWwindow * wnd, int32_t button, int32_t action, int32_t mod)
{
    window::m_windows[wnd]->sig_mouse_key().emit(button,action,mod);
}

void mouse_scroll_event(GLFWwindow * wnd, double sx, double sy)
{
    window::m_windows[wnd]->sig_mouse_scroll().emit(sx,sy);
}

void window_resize(GLFWwindow * wnd, int32_t w, int32_t h)
{
    window::m_windows[wnd]->sig_window_resized().emit(w,h);
}

void window_close(GLFWwindow * wnd)
{
    window::m_windows[wnd]->sig_window_closed().emit();
}

void key_event(GLFWwindow * wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
    window::m_windows[wnd]->sig_key_event().emit(key,scan_code,action,modifiers);
}

void text_event(GLFWwindow * wnd, uint32_t scan_code)
{
    window::m_windows[wnd]->sig_text_event().emit(scan_code);
}

void window::destroy_window(window * wnd)
{
    delete wnd;
    window::m_windows.erase(wnd->getWindow());
}
