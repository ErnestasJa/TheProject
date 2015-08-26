#pragma once

#include "IImageLoader.h"
// FIXME (Ernestas#1#): some pngs are not loaded, make sure they don't crash app

class Logger;
class png_loader: public iimage_loader
{
public:
    png_loader(Logger * l);
    virtual ~png_loader();

    virtual image_ptr load(void * buffer, const uint32_t size);
    virtual bool check_by_extension(const std::string & ext);

protected:
    Logger * _logger;
};
