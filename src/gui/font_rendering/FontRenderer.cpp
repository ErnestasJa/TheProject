#include "Precomp.h"
#include "Utility/helpers.h"
#include "opengl/Shader.h"
#include "opengl/OpenGLUtil.h"
#include "gui/GUIEnvironment.h"
#include "FontRenderer.h"
#include "Font.h"
#include <boost/algorithm/string.hpp>
#include <boost/regex.hpp>


font_renderer::font_renderer(GUIEnvironment* env)
{
    this->m_env=env;

    FT_Error err=FT_Init_FreeType(&ft);

    if (err)
    {
        printf("Error on init:%u\n",err);
        exit(-1);
    }

    set_default_font(create_font("default","res/freesans.ttf"));
    use_font();
    printf("Current font:%s\n",current_font->name.c_str());

    glGenVertexArrays(1,&vao);
    glGenBuffers(1,&vbo);
    font_shader=Shader::LoadShader("res/font");
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

void font_renderer::render_string(std::string font_name,std::wstring text,glm::vec2 pos,glm::vec4 color)
{
    use_font(font_name);

    glm::vec2 gs=m_env->get_gui_scale();
    float sx,sy; sx=gs.x; sy=gs.y;

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
	for (p = (const uint16_t *)text.c_str(); *p; p++) {
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

///tag nesting
///<c><b></b><c></c></c>
///
///if found any other similar <tag> after the initial <tag>, ending is not the first one </tag>.
///Continue to look past </tag>, found any other <tag>?
///yes: continue, no: this is the </tag> we are looking for. Get it's data and strip it.
///
///DO NOT newline inside tags(for now)(pls b0ss)
static void ReadAndStripTags(std::wstring str,vector<SubStrInfo> &ssi)
{
    bool dowehavetags=str.find_first_of(L"<")!=str.npos;
    bool alltagsdone=false;
    if(dowehavetags)
    {
        SubStrInfo inf;
        inf.bold=false;
        inf.col=glm::vec4(1);
        while(!alltagsdone)
        {
            if(str.find_first_of(L"<")!=0&&str.find_first_of(L"<")!=str.npos)
            {
                inf.text=str.substr(0,str.find_first_of(L"<"));
                ssi.push_back(inf);
                str=str.substr(inf.text.length());
                continue;
            }
            else if(str.find_first_of(L"<")==str.npos&&str.length()==0)
            {
                alltagsdone=true;
                break;
            }
            else if(str.length()!=0&&str.find_first_of(L"<")==str.npos)
            {
                inf.text=str;
                ssi.push_back(inf);
                alltagsdone=true;
                break;
            }

            wchar_t tag=str.substr(str.find_first_of(L"<")+1,1).c_str()[0];

            uint32_t tagstart=str.find_first_of(tag);
            uint32_t tagend=str.find_last_of(tag);

            switch(tag)
            {
            /// <c 255,255,255,255> </c>
            case L'c':
                const wchar_t *sr=str.substr(tagstart+1,3).c_str();
                const wchar_t *sg=str.substr(tagstart+5,3).c_str();
                const wchar_t *sb=str.substr(tagstart+9,3).c_str();
                const wchar_t *sa=str.substr(tagstart+13,3).c_str();

                glm::vec4 col=glm::vec4(1);
                float r,g,b;
                try
                {
                    r=1.f/255.f*_wtoi(sr);
                    g=1.f/255.f*_wtoi(sg);
                    b=1.f/255.f*_wtoi(sb);
                }
                catch(std::exception e)
                {
                    inf.col=col;
                }
                inf.col=glm::vec4(r,g,b,1);

                str=str.substr(tagstart+17,tagend-2);
                break;
            case L'b':
                inf.bold=true;
                str=str.substr(tagstart+17,tagend-2);
                break;
            default:
                break;
            }
        }
    }
}

/// Planned flow:
/// Split by newlines
/// Strip pre-tag text, strip after-tag text, strip and set tag info
/// Check for length wrapping failures, newline accordingly with the same data used for the formatted lines
/// Regex case: <([a-zA-Z][A-Z0-9]*)\b[^>]*>(.*?)</\1>
void font_renderer::render_string_formatted(std::wstring text, glm::vec2 pos,float linewidth,bool drawshadow)
{

    vector<std::wstring> strs;
    boost::split(strs, text, boost::is_any_of(L'\n'));

    vector<TextLine> linesToDraw;
    linesToDraw.resize(strs.size());

    SubLineInfo defaultinf;
    defaultinf.bold=false;
    defaultinf.col=glm::vec4(1);
    defaultinf.text=L"";

    /// For each new line of text, format it
    loop(i,strs.size())
    {
        uint32_t pretag=strs[i].find_first_of(L"<");
        uint32_t aftertag=strs[i].find_last_of(L"</")+2;
        std::wstring aftertagtext;
        if(pretag!=0)
        {
            defaultinf.text=strs[i].substr(0,pretag);
            linesToDraw[i].content.push_back(defaultinf);
            strs[i]=strs[i].substr(pretag);
        }
        if(aftertag!=npos)
        {
            aftertagtext=strs[i].substr(aftertag);
            strs[i]=strs[i].substr(0,aftertag);
        }
        ReadAndStripTags(strs[i],linesToDraw[i].content);
        if(aftertagtext.length()!=0)
        {
            defaultinf.text=aftertagtext;
            linesToDraw[i].content.push_back(defaultinf);
        }
    }

    loop(i,strs.size())
    {
        glm::vec2 substrdims=get_text_dimensions(strs[i],current_font->name);
        if(substrdims.x>linewidth)
        {

        }
        glm::vec2 linepos=glm::vec2(0,(float)i*(current_font->avgheight+current_font->avgheight/2));
        render_string(strs[i],pos+linepos,glm::vec4(1,1,1,1),true);
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
    for(uint16_t gl:text)
    {
        len+=a->c[gl].ax;
    }
    return glm::vec2(len,a->avgheight);
}
