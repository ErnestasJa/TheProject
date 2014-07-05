#include "Precomp.h"

#include "PNGLoader.h"
#include "stb_image.h"

#include "utility/Logger.h"

png_loader::png_loader(Logger * l): _logger(l)
{
    //ctor
}

png_loader::~png_loader()
{
    //dtor
}

image_ptr png_loader::load(void * buffer, const uint32_t size)
{
    int32_t x = 0, y=0,comp = 0;
    uint8_t * data = stbi_load_from_memory((const uint8_t*)buffer,size, &x, &y, &comp, 0);

    _logger->log(LOG_LOG, "Image loaded: w=%u; h=%u; comp=%u;", x, y, comp);

    if(data)
    {
        image * r = new image();
        r->data = data;
        r->num_channels = comp;
        r->width = x;
        r->height = y;
        return image_ptr(r);
    }

    return image_ptr(nullptr);
}

bool png_loader::check_by_extension(const std::string & ext)
{
    return ext == "png" || ext == ".png" || ext == "jpg" || ext == ".jpg";
}
