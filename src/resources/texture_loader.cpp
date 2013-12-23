#include "precomp.h"
#include "texture_loader.h"
#include "png_loader.h"
#include "tgaloader.h"

#include <algorithm>

texture_loader::texture_loader()
{
    add_loader(new png_loader());
    add_loader(new tgaloader());
}

texture_loader::~texture_loader()
{
    for(itexture_loader * l : m_loaders)
        delete l;
}


void        texture_loader::add_loader(itexture_loader * loader)
{
    auto it = std::find_if(m_loaders.begin(), m_loaders.end(), [&loader](itexture_loader * l){return l==loader;});

    if(it==m_loaders.end())
        m_loaders.push_back(loader);
}

texture *   texture_loader::load(const std::string & file)
{
    std::string ext = file.substr(file.find_last_of('.'));

    std::cout << "Image extension: '" << ext << "'." << std::endl;

    if(PHYSFS_exists(file.c_str()))
    for(itexture_loader * l : m_loaders)
    {
        if(l->check_by_extension(ext))
        {
            char * buf;
            uint32_t len = read(file,buf);

            if(len!=0)
            {
                std::cout << "Trying to load, file size: " << len << std::endl;
                return l->generate(buf,len);
            }
        }
    }

    return nullptr;
}
