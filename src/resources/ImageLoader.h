#pragma once

#include "iimage_loader.h"
#include "resource_cache.h"

struct texture; typedef std::shared_ptr<texture> texture_ptr;
class logger;
class image_loader: public resource_cache<image>
{
protected:
    logger * m_logger;
    std::vector<iimage_loader*> m_loaders;
public:
    image_loader(logger * l);
    virtual ~image_loader();

    virtual void add_loader(iimage_loader * loader);
    virtual image_ptr load(const std::string & file);
};
