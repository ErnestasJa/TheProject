#pragma once


using namespace tinyxml2;

enum gui_style
{
    gui_skin_whole_texture,

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
        rects.resize(gui_skin_style_count);
        uvs.resize(gui_skin_style_count*4);
    }

    void load(std::string filename)
    {
        std::string skin_path = filename.substr(0,filename.rfind("/")+1);
        std::cout << "skin_path =" << skin_path.c_str() << std::endl;

        tinyxml2::XMLDocument xml;
        uint32_t len;
        char * buf;
        len = helpers::read(filename, buf);

        if(len==0)
            return;

        xml.Parse(buf);

        XMLElement* root=xml.FirstChildElement();
        printf("Loading a skin %s (%s) %i\n",root->Attribute("name"),root->Attribute("atlas_name"),root->IntAttribute("atlas_size"));

        XMLElement* e=root->FirstChildElement();
        uint32_t i=1;
        while(e!=nullptr)
        {
            if(e->NoChildren())
            {
                rects[i]=Rect2D<int>(e->IntAttribute("x"),e->IntAttribute("y"),e->IntAttribute("w"),e->IntAttribute("h"));
                e=e->NextSiblingElement();
                i++;
                continue;
            }
            else
            {
                XMLElement* ce=e->FirstChildElement();
                while(ce!=nullptr)
                {
                    rects[i]=Rect2D<int>(ce->IntAttribute("x"),ce->IntAttribute("y"),ce->IntAttribute("w"),ce->IntAttribute("h"));
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

    void set_style_element(int32_t style,Rect2D<int> atlas_info)
    {
        if(style>gui_skin_style_count||style<0)
            return;

        rects[style]=atlas_info;
    }

    vector<glm::vec2> get_uv(uint32_t style)
    {
        vector<glm::vec2> ret;
        ret.resize(4);
        ret[0]=uvs[style*4];
        ret[1]=uvs[style*4+1];
        ret[2]=uvs[style*4+2];
        ret[3]=uvs[style*4+3];
        return ret;
    }

    glm::vec2 get_style_metrics(uint32_t style)
    {
        return glm::vec2(rects[style].w,rects[style].h);
    }

    void print()
    {
    }
private:
    vector<Rect2D<int> > rects;
    vector<glm::vec2> uvs;


    std::string name,atlas;
    uint32_t atlas_size;

    void generate_uv()
    {
        uvs[0]=glm::vec2(0,1);
        uvs[1]=glm::vec2(1,1);
        uvs[2]=glm::vec2(0,0);
        uvs[3]=glm::vec2(1,0);

        uint32_t j=4;
        for(int32_t i=1; i<gui_skin_style_count; i++)
        {
            Rect2D<float> ir=rects[i].as<float>();

            uvs[j+0]=glm::vec2(ir.x/256.f,ir.y2/256.f);
            uvs[j+1]=glm::vec2(ir.x2/256.f,ir.y2/256.f);
            uvs[j+2]=glm::vec2(ir.x2/256.f,ir.y/256.f);
            uvs[j+3]=glm::vec2(ir.x/256.f,ir.y/256.f);

            j+=4;
        }
    }
};
