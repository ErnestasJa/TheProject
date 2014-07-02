#pragma once
#include "StateManager.h"
class game_state
{
private:
public:
    game_state(state_manager* sm) { run=true; m_state_manager=sm; m_app_ctx=m_state_manager->get_app_context();};
    virtual ~game_state(){};

    virtual void on_load()=0;
    virtual void on_unload()=0;
    virtual void start()=0;
    virtual void update(float delta)=0;
    virtual void render()=0;

    void change_state(game_state* state)
    {
        m_state_manager->set_state(state);
    }
protected:
    bool run;
    state_manager* m_state_manager;
    app_context* m_app_ctx;
};
