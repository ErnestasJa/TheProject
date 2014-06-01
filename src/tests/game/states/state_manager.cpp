#include "precomp.h"
#include "../app_context.h"
#include "game_state.h"
#include "state_manager.h"
#include "application/application.h"

state_manager::state_manager(app_context* ctx)
{
    m_app_ctx=ctx;
    m_current_state=nullptr;
    run=true;
}

state_manager::~state_manager()
{
    if(m_current_state)
        m_current_state->on_unload();
}

void state_manager::set_state(game_state *state)
{
    if(m_current_state)
        m_current_state->on_unload();

    if(state==nullptr)
        return;

    m_current_state=state;
    state->on_load();
    state->start();
}

void state_manager::update(float delta)
{
    if(m_current_state)
    {
        m_current_state->update(delta);
    }
}

void state_manager::render()
{
    if(m_current_state)
    {
        m_current_state->render();
    }
}
