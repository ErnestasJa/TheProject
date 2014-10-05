#ifndef FONTFAMILY_H
#define FONTFAMILY_H

struct font;

enum FONT_FAMILY_TYPE
{
    FFT_REGULAR,
    FFT_BOLD,
    FFT_ITALIC,
    FFT_BOLD_ITALIC,

    FFT_COUNT
};

struct FontFamily
{
public:
    std::string familyName;
    FONT_FAMILY_TYPE currentType;
private:
    Font *_fonts[FFT_COUNT];
public:
    FontFamily(const std::string &name);
    std::string GetFamilyFontName(FONT_FAMILY_TYPE f);
    Font* GetFamilyFont(FONT_FAMILY_TYPE f);
    bool Has(FONT_FAMILY_TYPE f);
    void AddFont(FONT_FAMILY_TYPE f,Font* fnt);
};

#endif // FONTFAMILY_H
