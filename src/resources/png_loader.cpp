#include "precomp.h"

#include "png_loader.h"
#include "stb_image.h"

png_loader::png_loader()
{
    //ctor
}

png_loader::~png_loader()
{
    //dtor
}

image * png_loader::load(void * buffer, const uint32_t size)
{
    int32_t x = 0, y=0,comp = 0;
    uint8_t * data = stbi_load_from_memory((const uint8_t*)buffer,size, &x, &y, &comp, 0);

    std::cout << "Image loaded. w=" << x << "; h=" << y << "; "  << " comp=" << comp << ";"<< std::endl;

    if(data)
    {
        image * r = new image();
        r->data = data;
        r->num_channels = comp;
        r->width = x;
        r->height = y;
        return r;
    }

    return nullptr;
}

bool png_loader::check_by_extension(const std::string & ext)
{
    return ext == "png" || ext == ".png";
}
