#pragma once
template <class T>
class vec2;

class logger;
class input_handler
{
    public:
        input_handler(logger *log,GLFWwindow *win);
        virtual ~input_handler();

        glm::vec2 get_mouse_pos();
        bool mouse_button(int btn);
    protected:
    private:
        logger* log;
        glm::vec2 mouse_pos;
        GLFWwindow *window;
};
