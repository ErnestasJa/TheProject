#pragma once

#include "IImageLoader.h"
#include "ResourceCache.h"

struct Texture; typedef std::shared_ptr<Texture> TexturePtr;
class Logger;
class image_loader: public resource_cache<image>
{
protected:
    Logger * _logger;
    vector<iimage_loader*> m_loaders;
public:
    image_loader(Logger * l);
    virtual ~image_loader();

    virtual void add_loader(iimage_loader * loader);
    virtual image_ptr load(const std::string & file);
};
