#include "Precomp.h"
#include "Utility/helpers.h"
#include "opengl/Shader.h"
#include "opengl/OpenGLUtil.h"
#include "gui/GUIEnvironment.h"
#include "FontRenderer.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Font.h"
#include "FontFamily.h"
#include <boost/algorithm/string.hpp>


font_renderer::font_renderer(GUIEnvironment* env)
{
    this->m_env=env;

    FT_Error err=FT_Init_FreeType(&ft);

    if (err)
    {
        printf("Error on init:%u\n",err);
        exit(-1);
    }

    set_default_font(create_font("default","res/gui/fonts/OpenSans-Regular.ttf"));
    create_font("default-bold","res/gui/fonts/OpenSans-Bold.ttf");
    create_font("default-italic","res/gui/fonts/OpenSans-Italic.ttf");
    create_font("default-bolditalic","res/gui/fonts/OpenSans-BoldItalic.ttf");
    use_font();
    printf("Current font:%s\n",current_font->name.c_str());

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);
    font_shader=Shader::LoadShader("res/engine/shaders/font");
}

font_renderer::~font_renderer()
{
    for(fvi i=fonts.begin(); i!=fonts.end(); i++)
    {
        delete (*i);
    }
    fonts.clear();
    FT_Done_FreeType(ft);
}

font* font_renderer::create_font(std::string name, std::string filename, int32_t size)
{
    font* f=get_font(name);
    if(f!=nullptr)
    {
        return f;
    }

    char* buf=NULL;

    uint32_t fl=helpers::read(filename,buf);

    printf("BUF: %u\n",fl);

    if(fl<=0)
    {
        printf("Font file appears to be empty or corrupt.\n");
        exit(-1);
    }

    FT_Error err=FT_New_Memory_Face(ft, reinterpret_cast<FT_Byte*>(buf), fl, 0, &ff);
    if(err)
    {
        printf("Error on creating a font: %d\n",err);
        exit(-1);
    }

    font* temp=new font(ff, size, name);

    fonts.push_back(temp);

    delete buf;

    FT_Done_Face(ff);

    return temp;
}

bool font_renderer::remove_font(std::string font_to_remove)
{
    if(font_to_remove==default_font->name)
    {
        printf("Cannot remove default font.\n");
        return false;
    }

    for(fvi i=fonts.begin(); i!=fonts.end(); i++)
    {
        if((*i)->name==font_to_remove)
        {
            i=fonts.erase(i);
            delete (*i);
            return true;
        }
    }

    //if everything else fails.
    printf("fuck you.\n");
    return false;
}

void font_renderer::use_font(std::string font_name)
{
    if(font_name=="default"&&default_font!=nullptr)
    {
        current_font=default_font;
        return;
    }

    for(fvi i=fonts.begin(); i!=fonts.end(); i++)
    {
        if((*i)->name==font_name)
        {
            current_font=(*i);
            return;
        }
    }

    current_font=default_font;
    return;
}

font* font_renderer::get_font(std::string name)
{
    for(fvi i=fonts.begin(); i!=fonts.end(); i++)
    {
        if((*i)->name == name)
            return (*i);
    }

    return nullptr;
}

font* font_renderer::get_default_font()
{
    return default_font;
}

void font_renderer::set_default_font(font* new_font)
{
    default_font=new_font;
}

void font_renderer::render_string_internal(std::wstring text,glm::vec2 pos,glm::vec4 color)
{
    glm::vec2 gs=m_env->get_gui_scale();
    float sx,sy;
    sx=gs.x;
    sy=gs.y;

    pos.x=-1+pos.x*sx;
    pos.y=1-pos.y*sy-current_font->avgheight*sy;

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    font_shader->Set();
    set_font_color(color);

    const uint16_t *p;

    /* Use the Texture containing the atlas */
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, current_font->tex));

    glBindVertexArray(vao);

    /* Set up the VBO for our vertex data */
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0));

    glm::vec4 coords[6 * text.length()];
    int c = 0;

    font* a=current_font;

    /* Loop through all characters */
    for (p = (const uint16_t *)text.c_str(); *p; p++)
    {
        /* Calculate the vertex and Texture coordinates */
        float x2 = pos.x + a->c[*p].bl * sx;
        float y2 = -pos.y - a->c[*p].bt * sy;
        float w = a->c[*p].bw * sx;
        float h = a->c[*p].bh * sy;

        /* Advance the cursor to the start of the next character */
        pos.x += a->c[*p].ax * sx;
        pos.y += a->c[*p].ay * sy;

        /* Skip glyphs that have no pixels */
        if (!w || !h)
            continue;

        coords[c++] = glm::vec4(x2, -y2, a->c[*p].tx, a->c[*p].ty);
        coords[c++] = glm::vec4(x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h);
        coords[c++] = glm::vec4(x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty);

        coords[c++] = glm::vec4(x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h);
        coords[c++] = glm::vec4(x2 + w, -y2 - h, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h);
        coords[c++] = glm::vec4(x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty);
    }

    /* Draw all the character on the screen in one go */
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW));
    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, c));

    GL_CHECK(glDisableVertexAttribArray(0));
    glBindVertexArray(0);

    glDisable(GL_BLEND);
}

void font_renderer::render_string(std::string fontname,std::wstring text,glm::vec2 pos,glm::vec4 color,bool drawshadow)
{
    std::string oldfont=current_font->name;
    use_font(fontname);
    if(drawshadow)
        render_string_internal(text,glm::vec2(pos.x+1,pos.y+1),glm::vec4(0,0,0,color.w));
    render_string_internal(text,pos,color);
    use_font(oldfont);
}

void font_renderer::render_string(std::wstring text,glm::vec2 pos,glm::vec4 color,bool drawshadow)
{
    if(drawshadow)
        render_string(current_font->name,text,glm::vec2(pos.x+1,pos.y+1),glm::vec4(0,0,0,color.w));
    render_string(current_font->name,text,pos,color);
}

void font_renderer::render_string(std::wstring text, glm::vec2 pos,bool drawshadow)
{
    if(drawshadow)
        render_string(current_font->name,text,glm::vec2(pos.x+1,pos.y+1),glm::vec4(0,0,0,1));
    render_string(current_font->name,text,pos,glm::vec4(1,1,1,1));
}

static uint32_t FindTagEnd(std::wstring str,const wchar_t tag)
{
    wchar_t buf[32];

    swprintf(buf,L"['%c",tag);
    std::wstring thistagopen=buf;

    swprintf(buf,L"[%c']",tag);
    std::wstring thistagclose=buf;

    uint32_t tagstart=str.find(thistagopen);

    uint32_t naiveend=str.find(thistagclose,tagstart);

    uint32_t track=tagstart;
    uint32_t realend=naiveend;
    while(1)
    {
        uint32_t nestedcheck=str.find(thistagopen,track+3);
        bool nested=nestedcheck!=std::wstring::npos;

        if(nested&&nestedcheck<naiveend)
        {
            track=str.find(thistagopen,track+3);
            naiveend=str.find(thistagclose,naiveend+4);
        }
        else
            break;
    }
    realend=naiveend;
    return realend;
}

static void FormatTags(TextLine &tl,std::wstring in,SubLineInfo inf)
{
    if(in.length()==0)return;
    uint32_t firsttag,tagclose,tagend,taglength;
    wchar_t tag;

    firsttag=in.find(L"['");

    if(firsttag!=std::wstring::npos)
    {
        ///printf("tag found\n");
        tag=in.substr(firsttag+2,1)[0];
        tagclose=in.find(L"]",firsttag);
        tagend=FindTagEnd(in,tag);
        taglength=tagend-(tagclose+1);
    }
    else /// apparently, no tags
    {
        ///printf("no tag found\n");
        if(in.length()>0)
        {
            inf.text=in;
            tl.content.push_back(inf);
        }
        return;
    }
    /// do we have pre-text? if yes, cut it and try again. keep formatting data.
    if(firsttag!=0)
    {
        inf.text=in.substr(0,firsttag);
        tl.content.push_back(inf);
        in=in.substr(firsttag);
        ///wprintf(L"pretag found applying formatting if any\nsubstr:%ls\n",in.c_str());
        FormatTags(tl,in,inf);
        return;
    }

    SubLineInfo oldinf=inf;
    switch(tag)
    {
    case L'c':
    {
        std::vector<std::wstring> tagvals;
        std::wstring tagvalsubstr=in.substr(firsttag+4,tagclose-4);
        boost::split(tagvals,tagvalsubstr,boost::is_any_of(L","));

        float r,g,b,a;
        r=1.f/255.f*_wtoi(tagvals[0].c_str());
        g=1.f/255.f*_wtoi(tagvals[1].c_str());
        b=1.f/255.f*_wtoi(tagvals[2].c_str());
        a=1.f/255.f*_wtoi(tagvals[3].c_str());
        inf.color=glm::vec4(r,g,b,a);
    }
    break;
    case L's':
    {
        inf.shadow=true;
    }
    break;
    case L'b':
    {
        inf.bold=true;
    }
    break;
    case L'i':
    {
        inf.italic=true;
    }
    break;
    case L'v':
    {
        inf.bold=true;
        inf.italic=true;
    }
    break;
    }

    std::wstring before=in.substr(tagclose+1,taglength);
    std::wstring after=in.substr(tagend+4);
    in=before+after;
    FormatTags(tl,before,inf);
    FormatTags(tl,after,oldinf);
    return;
}

/// Planned flow:
/// Split by newlines
/// Strip pre-tag text, strip after-tag text, strip and set tag info
/// Check for length wrapping failures, newline accordingly with the same data used for the formatted lines
/// Regex case: <([a-zA-Z][A-Z0-9]*)\b[^>]*>(.*?)</\1>
void font_renderer::render_string_formatted(std::wstring text, glm::vec2 pos,float linewidth,bool drawshadow)
{
    std::string oldfont;

    char boldname[256];
    sprintf(boldname,"%s-bold",current_font->name.c_str());
    font* boldie=get_font(boldname);
    bool canbold=boldie!=nullptr;

    char italicname[256];
    sprintf(italicname,"%s-italic",current_font->name.c_str());
    font* italie=get_font(italicname);
    bool canitalic=italie!=nullptr;

    char bolditalicname[256];
    sprintf(bolditalicname,"%s-bolditalic",current_font->name.c_str());
    font* boldieitalie=get_font(bolditalicname);
    bool canbolditalic=boldieitalie!=nullptr;


    vector<std::wstring> strs;
    boost::split(strs, text, boost::is_any_of(L"\n"));
    if(strs.size()==0)
        strs.push_back(text);

    vector<TextLine> linesToDraw;
    linesToDraw.resize(strs.size());

    SubLineInfo inf;
    inf.color=glm::vec4(1);
    inf.shadow=false;
    inf.bold=false;
    inf.italic=false;

    loop(i,strs.size())
    {
        FormatTags(linesToDraw[i],strs[i],inf);
    }

    //ReadAndStripTags(strs,linesToDraw);
    //printf("LinesToDraw: %d\n",linesToDraw.size());
    loop(i,linesToDraw.size())
    {
        glm::vec2 dims=glm::vec2(0,current_font->avgheight);
        TextLine _current=linesToDraw[i];
        loop(j,_current.content.size())
        {
            SubLineInfo _celem=_current.content[j];
            if(_celem.bold&&canbold&&!_celem.italic)
            {
                oldfont=current_font->name;
                use_font(std::string(boldname));
            }
            else if(_celem.italic&&canitalic&&!_celem.bold)
            {
                oldfont=current_font->name;
                use_font(std::string(italicname));
            }
            else if(_celem.bold&&_celem.italic&&canbolditalic)
            {
                oldfont=current_font->name;
                use_font(std::string(bolditalicname));
            }
            if(j!=0)
                render_string(current_font->name,_celem.text,pos+glm::vec2(dims.x,i*(dims.y+dims.y/2.f)),_celem.color,_celem.shadow);
            else
                render_string(current_font->name,_celem.text,pos+glm::vec2(0,i*(dims.y+dims.y/2.f)),_celem.color,_celem.shadow);
            dims=glm::vec2(dims.x+get_text_dimensions(_celem.text,current_font->name).x,dims.y);
            if(_celem.bold||_celem.italic)
            {
                use_font(oldfont);
            }
        }
    }
}

void font_renderer::set_font_color(glm::vec4 color)
{
    glUniform4fv(font_shader->getparam("color"),1,glm::value_ptr(color));
}

glm::vec2 font_renderer::get_text_dimensions(const std::wstring & text,const std::string &font_name)
{
    font* a=get_font(font_name);

    int len=0;
    for(wchar_t gl:text)
    {
        len+=a->c[(int)gl].ax;
    }
    return glm::vec2(len,a->avgheight);
}

