#include "precomp.h"

#include "image_loader.h"
#include "png_loader.h"
#include "tgaloader.h"

#include "utility/logger.h"

image_loader::image_loader(logger * l): m_logger(l)
{
    add_loader(new png_loader(l));
    add_loader(new tgaloader(l));
}

image_loader::~image_loader()
{
    for(iimage_loader * l : m_loaders)
        delete l;
}


void        image_loader::add_loader(iimage_loader * loader)
{
    auto it = std::find_if(m_loaders.begin(), m_loaders.end(), [&loader](iimage_loader * l){return l==loader;});

    if(it==m_loaders.end())
        m_loaders.push_back(loader);
}

image_ptr image_loader::load(const std::string & file)
{
    resource<image> res;
    res = this->get_resource(file);

    if(res.resource)
    {
        m_logger->log(LOG_LOG, "Found image in cache, skipping loading.");
        return res.resource;
    }

    std::string ext = file.substr(file.find_last_of('.'));
    m_logger->log(LOG_LOG, "Image extension: '%s'", ext.c_str());

    if(PHYSFS_exists(file.c_str()))
    for(iimage_loader * l : m_loaders)
    {
        if(l->check_by_extension(ext))
        {
            char * buf;
            uint32_t len = read(file,buf);

            if(len!=0)
            {
                m_logger->log(LOG_LOG, "Image file size: %u", len);

                res.path = file;
                res.resource = image_ptr(l->load(buf,len));
                this->add_resource(res);
                return res.resource;
            }
        }
    }

    return nullptr;
}
