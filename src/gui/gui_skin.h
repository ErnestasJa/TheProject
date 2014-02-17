#pragma once

using namespace tinyxml2;

enum gui_style
{
    gui_skin_background,

    gui_skin_input_active,
    gui_skin_input_hover,
    gui_skin_input_disabled,

    gui_skin_button_active,
    gui_skin_button_hover,
    gui_skin_button_click,
    gui_skin_button_disabled,

    gui_skin_checkbox_u_active,
    gui_skin_checkbox_u_hover,
    gui_skin_checkbox_u_click,
    gui_skin_checkbox_u_disabled,

    gui_skin_checkbox_c_active,
    gui_skin_checkbox_c_hover,
    gui_skin_checkbox_c_click,
    gui_skin_checkbox_c_disabled,

    gui_skin_radio_u_active,
    gui_skin_radio_u_hover,
    gui_skin_radio_u_click,
    gui_skin_radio_u_disabled,

    gui_skin_radio_c_active,
    gui_skin_radio_c_hover,
    gui_skin_radio_c_click,
    gui_skin_radio_c_disabled,

    gui_skin_progress_h_bg,
    gui_skin_progress_h_bar,

    gui_skin_scroll_h_bg,
    gui_skin_scroll_h_bar,

    gui_skin_scroll_v_bg,
    gui_skin_scroll_v_bar,

    gui_skin_titlebar,

    gui_skin_color_text,
    gui_skin_color_text_disabled,

    gui_skin_style_count
};

struct gui_skin
{
    gui_skin()
    {
    }

    void load(std::string filename)
    {
        XMLDocument xml;
        xml.LoadFile(filename.c_str());

        XMLElement* root=xml.FirstChildElement();
        printf("Loading a skin %s (%s) %i\n",root->Attribute("name"),root->Attribute("atlas_name"),root->IntAttribute("atlas_size"));

        XMLElement* e=root->FirstChildElement();
        uint32_t i=0;
        while(e!=nullptr)
        {
            if(e->NoChildren())
            {
                rects[i]=rect2d<int>(e->IntAttribute("x"),e->IntAttribute("y"),e->IntAttribute("w"),e->IntAttribute("h"));
                e=e->NextSiblingElement();
                i++;
                continue;
            }
            else
            {
                XMLElement* ce=e->FirstChildElement();
                while(ce!=nullptr)
                {
                    rects[i]=rect2d<int>(ce->IntAttribute("x"),ce->IntAttribute("y"),ce->IntAttribute("w"),ce->IntAttribute("h"));
                    ce=ce->NextSiblingElement();
                    i++;
                }
                e=e->NextSiblingElement();
                continue;
            }
        }
        generate_uv();
        xml.Clear();
    }

    bool check_completeness()
    {
        return true;
    }

    void save(std::string filename)
    {
        if(check_completeness())
        {
            generate_uv();
            //saving here
        }
        else
            return;
    }

    void set_style_element(int32_t style,rect2d<int> atlas_info)
    {
        if(style>gui_skin_style_count||style<0)
            return;

        rects[style]=atlas_info;
    }

    glm::vec2* get_uv(uint32_t style)
    {
        return new glm::vec2[4]{uvs[style*4],uvs[style*4+1],uvs[style*4+2],uvs[style*4+3]};
    }

    void print()
    {
        for(uint32_t i=0; i<gui_skin_style_count; i++)
        {
            printf("%s",rects[i].to_string().c_str());
        }
    }
private:
    rect2d<int> rects[gui_skin_style_count];
    glm::vec2 uvs[gui_skin_style_count*4];

    std::string name,atlas;
    uint32_t atlas_size;

    void generate_uv()
    {
        uint32_t j=0;
        for(int32_t i=0; i<gui_skin_style_count; i++)
        {
            rect2d<float> ir=rects[i].as<float>();

            uvs[j+3]=glm::vec2(ir.x/256.f,ir.y/256.f);
            uvs[j+2]=glm::vec2(ir.x2/256.f,ir.y/256.f);
            uvs[j+1]=glm::vec2(ir.x2/256.f,ir.y2/256.f);
            uvs[j]=glm::vec2(ir.x/256.f,ir.y2/256.f);

            j+=4;
        }
    }
};
