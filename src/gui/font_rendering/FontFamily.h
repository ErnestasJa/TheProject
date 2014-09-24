#ifndef FONTFAMILY_H
#define FONTFAMILY_H

struct font;

enum FONT_FAMILY
{
    FF_REGULAR,
    FF_BOLD,
    FF_ITALIC,
    FF_BOLD_ITALIC,

    FF_COUNT
};

struct FontFamily
{
public:
    std::string familyName;
private:
    font *fonts[FF_COUNT];
public:
    std::string GetFamilyFontName(FONT_FAMILY f);
    font* GetFamilyFont(FONT_FAMILY f);
    bool Has(FONT_FAMILY f);
    void AddFont(FONT_FAMILY f,font* fnt);
};

#endif // FONTFAMILY_H
