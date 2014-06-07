#pragma once

class game_state;
class application;
struct app_context;
class state_manager
{
private:
    app_context* m_app_ctx;
    game_state* m_current_state;
public:
    state_manager(app_context* ctx);
    ~state_manager();

    void set_state(game_state* state);
    //void push_state(game_state* state);
    //void pop_state();

    void update(float delta);
    void render();

    bool run;
    void exit(){run=false;}

    app_context* get_app_context(){return m_app_ctx;}
protected:
};
