#pragma once

#include "iimage_loader.h"

class image_loader
{
protected:
    std::vector<iimage_loader*> m_loaders;
public:
    image_loader();
    virtual ~image_loader();

    virtual void add_loader(iimage_loader * loader);
    virtual image * load(const std::string & file);
};
