#pragma once
template <class T>
class vec2;

class logger;
class input_handler
{
    public:
        input_handler(logger *log);
        virtual ~input_handler();

        vec2<int> get_mouse_pos();
        bool mouse_button(int btn);
    protected:
    private:
        logger* log;
        vec2<int> mouse_pos;
};
