#include "Precomp.h"
#include "../../AppContext.h"
#include "GameState.h"
#include "StateManager.h"
#include "Application/Application.h"

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
    {
        m_current_state->on_unload();
        m_app_ctx->env->destroy_children();
        m_app_ctx->env->update(0);
        m_app_ctx->sg->clear();
        m_app_ctx->sg->update_all(0);
        m_app_ctx->pm->clear();
        m_app_ctx->pm->update(0);
    }
    m_current_state=nullptr;

    if(state==nullptr)
        return;

    m_current_state=state;
    state->on_load();
    state->start();
}

void state_manager::update(float delta)
{
    if(m_app_ctx->nm->should_deinit)
        m_app_ctx->nm->deinit();
    if(m_current_state!=nullptr)
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
