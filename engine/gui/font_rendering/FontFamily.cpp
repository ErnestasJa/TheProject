#include "Precomp.h"
#include "ft2build.h"
#include FT_FREETYPE_H
#include "Font.h"
#include "FontFamily.h"

FontFamily::FontFamily(const std::string &name)
{
    _fonts[FFT_REGULAR]=nullptr;
    _fonts[FFT_BOLD]=nullptr;
    _fonts[FFT_ITALIC]=nullptr;
    _fonts[FFT_BOLD_ITALIC]=nullptr;
    familyName=name;
}

std::string FontFamily::GetFamilyFontName(FONT_FAMILY_TYPE f)
{
    if(Has(f)) return _fonts[f]->name;
    else return std::string("___INVALID___");
}
Font* FontFamily::GetFamilyFont(FONT_FAMILY_TYPE f)
{
    if(Has(f)) return _fonts[f];
    else return nullptr;
}
bool FontFamily::Has(FONT_FAMILY_TYPE f)
{
    return _fonts[f]!=nullptr;
}
void FontFamily::AddFont(FONT_FAMILY_TYPE f,Font* fnt)
{
    if(!Has(f)) _fonts[f]=fnt;
}
