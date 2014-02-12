#include "precomp.h"

#include "opengl/shader.h"
#include "opengl/opengl_util.h"
#include "gui/gui_environment.h"
#include "font_renderer.h"
#include "font.h"


font_renderer::font_renderer(gui_environment* env)
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

    font_shader=shader::load_shader("res/font");
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

void font_renderer::render_string(std::string font_name,std::string text,glm::vec2 pos,glm::vec4 color)
{
    use_font(font_name);

    glm::vec2 gs=m_env->get_gui_scale();
    float sx,sy; sx=gs.x; sy=gs.y;

    pos.x=-1+pos.x*sx;
    pos.y=1-pos.y*sy-(float)((float)(current_font->avgheight)*sy);

    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    font_shader->set();
    set_font_color(color);

    const uint8_t *p;

	/* Use the texture containing the atlas */
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
	for (p = (const uint8_t *)text.c_str(); *p; p++) {
		/* Calculate the vertex and texture coordinates */
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
		coords[c++] = glm::vec4(x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty);
		coords[c++] = glm::vec4(x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h);
		coords[c++] = glm::vec4(x2 + w, -y2, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty);
		coords[c++] = glm::vec4(x2, -y2 - h, a->c[*p].tx, a->c[*p].ty + a->c[*p].bh / a->h);
		coords[c++] = glm::vec4(x2 + w, -y2 - h, a->c[*p].tx + a->c[*p].bw / a->w, a->c[*p].ty + a->c[*p].bh / a->h);
	}

	/* Draw all the character on the screen in one go */
	GL_CHECK(glBufferData(GL_ARRAY_BUFFER, sizeof coords, coords, GL_DYNAMIC_DRAW));
	GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, c));

	GL_CHECK(glDisableVertexAttribArray(0));
	glBindVertexArray(0);

	glDisable(GL_BLEND);
}

void font_renderer::render_string(std::string text,glm::vec2 pos,glm::vec4 color)
{
    render_string(default_font->name,text,pos,color);
}

void font_renderer::render_string(std::string text, glm::vec2 pos)
{
    render_string(default_font->name,text,pos,glm::vec4(1,1,1,1));
}

void font_renderer::set_font_color(glm::vec4 color)
{
    glUniform4fv(font_shader->getparam("color"),1,glm::value_ptr(color));
}

glm::vec2 font_renderer::get_text_dimensions(const std::string & text,const std::string &font_name)
{
    font* a=get_font(font_name);

    int len=0;
    int maxh=0;
    for(char gl:text)
    {
        len+=a->c[gl].bw;
        if(a->c[gl].bh>maxh)
            maxh=a->h;
    }
    return glm::vec2(len,maxh);
}
