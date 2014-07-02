#pragma once

#include "IImageLoader.h"
// FIXME (Ernestas#1#): some pngs are not loaded, make sure they don't crash app

class logger;
class png_loader: public iimage_loader
{
public:
    png_loader(logger * l);
    virtual ~png_loader();

    virtual image_ptr load(void * buffer, const uint32_t size);
    virtual bool check_by_extension(const std::string & ext);

protected:
    logger * m_logger;
};
