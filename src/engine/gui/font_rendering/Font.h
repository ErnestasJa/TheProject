#pragma once

#define MAX_FONT_ATLAS_WIDTH 1024
#define MAX_FONT_ATLAS_HEIGHT 1024

struct FontFamily;

struct Font
{
    FontFamily *myFamily;

    std::string name;

    GLuint tex;		// Texture object

    int w;			// width of Texture in pixels
    int h;			// height of Texture in pixels
    float avgheight;

    struct
    {
        float ax;	// advance.x
        float ay;	// advance.y

        float bw;	// bitmap.width;
        float bh;	// bitmap.height;

        float bl;	// bitmap_left;
        float bt;	// bitmap_top;

        float tx;	// x offset of glyph in Texture coordinates
        float ty;	// y offset of glyph in Texture coordinates
    } c[1024];		// character information

    Font(FT_Face face, int height, std::string name);

    ~Font();
};
