#include "Precomp.h"
#include "utility/Vector.h"
#include "utility/Rect2d.h"
#include "GUISkin.h"

GUISkin::GUISkin()
{
    rects.resize(gui_skin_style_count);
    uvs.resize(gui_skin_style_count*4);
    margins.resize(gui_skin_style_count);
}

void GUISkin::load(std::string filename)
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
            if(e->QueryAttribute("m",&margins[i])==XML_NO_ATTRIBUTE)printf("WTF!");
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
                if(ce->QueryAttribute("m",&margins[i])==XML_NO_ATTRIBUTE)printf("WTF!");
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

void GUISkin::set_style_element(int32_t style,Rect2D<int> atlas_info)
{
    if(style>gui_skin_style_count||style<0)
        return;

    rects[style]=atlas_info;
}

vector<glm::vec2> GUISkin::get_uv(uint32_t style)
{
    vector<glm::vec2> ret;
    ret.resize(4);
    ret[0]=uvs[style*4];
    ret[1]=uvs[style*4+1];
    ret[2]=uvs[style*4+2];
    ret[3]=uvs[style*4+3];
    return ret;
}

void GUISkin::generate_uv()
{
    uvs[0]=glm::vec2(0,0);
    uvs[1]=glm::vec2(1,0);
    uvs[2]=glm::vec2(1,1);
    uvs[3]=glm::vec2(0,1);

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
