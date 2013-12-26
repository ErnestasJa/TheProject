#include "precomp.h"

#include "image_loader.h"
#include "png_loader.h"
#include "tgaloader.h"

image_loader::image_loader()
{
    add_loader(new png_loader());
    add_loader(new tgaloader());
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

image *   image_loader::load(const std::string & file)
{
    std::string ext = file.substr(file.find_last_of('.'));

    std::cout << "Image extension: '" << ext << "'." << std::endl;

    if(PHYSFS_exists(file.c_str()))
    for(iimage_loader * l : m_loaders)
    {
        if(l->check_by_extension(ext))
        {
            char * buf;
            uint32_t len = read(file,buf);

            if(len!=0)
            {
                std::cout << "Trying to load, file size: " << len << std::endl;
                return l->load(buf,len);
            }
        }
    }

    return nullptr;
}
