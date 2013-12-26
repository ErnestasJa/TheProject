#pragma once

#define MAX_FONT_ATLAS_WIDTH 1024
#define MAX_FONT_ATLAS_HEIGHT 1024
struct font
{
    std::string name;

    GLuint tex;		// texture object

    int w;			// width of texture in pixels
    int h;			// height of texture in pixels
    float avgheight;

    struct
    {
        float ax;	// advance.x
        float ay;	// advance.y

        float bw;	// bitmap.width;
        float bh;	// bitmap.height;

        float bl;	// bitmap_left;
        float bt;	// bitmap_top;

        float tx;	// x offset of glyph in texture coordinates
        float ty;	// y offset of glyph in texture coordinates
    } c[128];		// character information

    font(FT_Face face, int height, std::string name)
    {
        this->avgheight=height;
        this->name=name;

        FT_Set_Pixel_Sizes(face, 0, height);
        FT_GlyphSlot g = face->glyph;

        int roww = 0;
        int rowh = 0;
        w = 0;
        h = 0;

        memset(c, 0, sizeof c);
        /* Find minimum size for a texture holding all visible ASCII characters */
        for (int i = 32; i < 128; i++)
        {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            {
                fprintf(stderr, "Loading character %c failed!\n", i);
                continue;
            }
            if (roww + g->bitmap.width + 1 >= MAX_FONT_ATLAS_WIDTH)
            {
                w = std::max(w, roww);
                h += rowh;
                roww = 0;
                rowh = 0;
            }
            roww += g->bitmap.width + 1;
            rowh = std::max(rowh, g->bitmap.rows);
        }

        w = std::max(w, roww);
        h += rowh;

        /* Create a texture that will be used to hold all ASCII glyphs */
        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_BGR, GL_UNSIGNED_BYTE, 0);

        /* We require 1 byte alignment when uploading texture data */
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        /* Clamping to edges is important to prevent artifacts when scaling */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        /* Linear filtering usually looks best for text */
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        /* Paste all glyph bitmaps into the texture, remembering the offset */
        int ox = 0;
        int oy = 0;

        rowh = 0;
        int cnth=0;
        for (int i = 32; i < 128; i++)
        {
            if (FT_Load_Char(face, i, FT_LOAD_RENDER))
            {
                fprintf(stderr, "Loading character %c failed!\n", i);
                continue;
            }
            avgheight+=g->bitmap.rows;
            cnth++;
            if (ox + g->bitmap.width + 1 >= MAX_FONT_ATLAS_WIDTH)
            {
                oy += rowh;
                rowh = 0;
                ox = 0;
            }
            glTexSubImage2D(GL_TEXTURE_2D, 0, ox, oy, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);
            c[i].ax = g->advance.x >> 6;
            c[i].ay = g->advance.y >> 6;

            c[i].bw = g->bitmap.width;
            c[i].bh = g->bitmap.rows;

            c[i].bl = g->bitmap_left;
            c[i].bt = g->bitmap_top;

            c[i].tx = ox / (float)w;
            c[i].ty = oy / (float)h;

            rowh = std::max(rowh, g->bitmap.rows);
            ox += g->bitmap.width + 1;
        }
        avgheight=avgheight/(float)cnth;
        fprintf(stderr, "Generated a %d x %d (%d kb) texture atlas\n", w, h, w * h / 1024);
    }

    ~font()
    {
        glDeleteTextures(1, &tex);
    }
};
