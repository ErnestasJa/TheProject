#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H


struct KeyInfo
{
    uint32_t key;
    uint32_t *mods;
};

class Window;

class InputHandler
{
    public:
        InputHandler(Window* win);
        virtual ~InputHandler();

        bool IsKeyDown(uint32_t keyCode);
        bool IsKeyUp(uint32_t keyCode);

        bool IsMouseDown(uint32_t mouseKey);
        bool IsMouseUp(uint32_t mouseKey);
    protected:
        bool keys[GLFW_KEY_LAST];
        bool mouseKeys[GLFW_MOUSE_BUTTON_LAST];

        void OnMouseButton(int32_t button, int32_t action, int32_t mod);
        void OnKey(int32_t key, int32_t scanCode, int32_t action, int32_t mod);
    private:
        sigc::connection _sig_mouse_button,_sig_key;
};

#endif // INPUTHANDLER_H
