#pragma once
#include "../app_context.h"
#include "game_state.h"
#include "gui/gui_environment.h"
#include "gui/gui_static_text.h"
#include "gui/font_renderer.h"

class default_game_state:public game_state
{
private:
    gui_static_text* message;
    gui_environment* m_env;
    std::wstring m_message;
public:
    default_game_state(state_manager* sm,std::wstring message):game_state(sm)
    {
        m_env=m_app_ctx->env;
        m_message=message;
    }

    virtual ~default_game_state()
    {

    }

    void on_unload()
    {

    }

    void on_load()
    {

    }

    void start()
    {

    }

    void update(float delta)
    {

    }

    void render()
    {
        glClearColor(0,0,0,1);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

        m_env->get_font_renderer()->render_string(m_message,glm::vec2(500,360),glm::vec4(1,1,1,1),false);
    }
protected:
};
