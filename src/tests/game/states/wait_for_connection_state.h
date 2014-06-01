#pragma once
#include "../app_context.h"
#include "game_state.h"
#include "gui/gui_environment.h"
#include "gui/gui_image.h"
#include "gui/font_renderer.h"

#include "menu_state.h"

class wait_for_connection_state:public game_state
{
private:
    gui_environment* m_env;
    std::wstring m_message;
public:
    wait_for_connection_state(state_manager* sm):game_state(sm)
    {
        m_env=m_app_ctx->env;
    }

    virtual ~wait_for_connection_state()
    {

    }

    void on_unload()
    {
    }

    void on_load()
    {
        new gui_image(m_env,rect2d<int>(384,256,512,256),m_app_ctx->gm->load_texture("res/logo_quad2.png"));
        printf("Network manager should init here\n");
        m_app_ctx->nm->init();
    }

    void start()
    {

    }

    void update(float delta)
    {
        m_env->update(delta);
        if(m_app_ctx->nm->is_receiving())
            m_state_manager->set_state(new menu_state(m_state_manager));
    }

    void render()
    {
        glClearColor(0,0,0,1);
        glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);

        m_env->render();
        m_env->get_font_renderer()->render_string(L"Laukiame prisijungimo...",glm::vec2(512+256-m_env->get_font_renderer()->get_text_dimensions(L"Laukiame prisijungimo...").x,530),glm::vec4(1,1,1,1),false);
    }
protected:
};
