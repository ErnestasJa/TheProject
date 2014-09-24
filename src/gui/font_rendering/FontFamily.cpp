#include "Precomp.h"
#include <ft2build.h>
#include FT_FREETYPE_H
#include "Font.h"
#include "FontFamily.h"

std::string FontFamily::GetFamilyFontName(FONT_FAMILY f)
{
    if(Has(f)) return fonts[f]->name;
    else return std::string("___INVALID___");
}
font* FontFamily::GetFamilyFont(FONT_FAMILY f)
{
    if(Has(f)) return fonts[f];
    else return nullptr;
}
bool FontFamily::Has(FONT_FAMILY f)
{
    return fonts[f]!=nullptr;
}
void FontFamily::AddFont(FONT_FAMILY f,font* fnt)
{
    if(!Has(f)) fonts[f]=fnt;
}
