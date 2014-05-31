#include "precomp.h"
#include "../app_context.h"
#include "game_state.h"
#include "state_manager.h"
#include "application/application.h"
#include "default_game_state.h"

state_manager::state_manager(app_context* ctx)
{
    m_app_ctx=ctx;
    m_current_state=nullptr;
}

state_manager::~state_manager()
{

}

void state_manager::set_state(game_state *state)
{
    if(state==nullptr)
        return;
    m_current_state=state;
    state->on_load();
    state->start();
}

void state_manager::update(float delta)
{
    if(m_current_state!=nullptr)
    {
        m_current_state->update(delta);
    }
}

void state_manager::render()
{
    if(m_current_state!=nullptr)
    {
        m_current_state->render();
    }
}
