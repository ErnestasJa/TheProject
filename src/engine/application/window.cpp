#include "Precomp.h"

#include "Window.h"

std::map<GLFWwindow*, Window*>   Window::_windows;

void MouseMoveEvent(GLFWwindow * wnd, double x, double y)
{
    Window::_windows[wnd]->SigMouseMoved().emit(x,y);
}

void MouseKeyEvent(GLFWwindow * wnd, int32_t button, int32_t action, int32_t mod)
{
    Window::_windows[wnd]->SigMouseKey().emit(button,action,mod);
}

void MouseScrollEvent(GLFWwindow * wnd, double sx, double sy)
{
    Window::_windows[wnd]->SigMouseScroll().emit(sx,sy);
}

void WindowResize(GLFWwindow * wnd, int32_t w, int32_t h)
{
    Window::_windows[wnd]->SigWindowResized().emit(w,h);
}

void WindowClose(GLFWwindow * wnd)
{
    Window::_windows[wnd]->SigWindowClosed().emit();
    Window::_windows[wnd]->_shouldClose = true;

}

void KeyEvent(GLFWwindow * wnd, int32_t key, int32_t scan_code, int32_t action, int32_t modifiers)
{
    Window::_windows[wnd]->SigKeyEvent().emit(key,scan_code,action,modifiers);
}

void TextEvent(GLFWwindow * wnd, uint32_t scan_code)
{
    Window::_windows[wnd]->SigTextEvent().emit(scan_code);
}

void Window::DestroyWindow(Window * wnd)
{
    delete wnd;
    Window::_windows.erase(wnd->getWindow());
}
