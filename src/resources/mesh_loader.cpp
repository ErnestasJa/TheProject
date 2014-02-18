#include "precomp.h"
#include "mesh_loader.h"
#include "iqmloader.h"
#include "opengl/mesh.h"
#include "utility/logger.h"

mesh_loader::mesh_loader(logger * l)
{
    m_logger = l;
    add_loader(new iqmloader(l));
}

mesh_loader::~mesh_loader()
{
    for(imesh_loader * l : m_loaders)
        delete l;
}

void mesh_loader::add_loader(imesh_loader * loader)
{
    auto it = std::find_if(m_loaders.begin(), m_loaders.end(), [&loader](imesh_loader * l){return l==loader;});

    if(it==m_loaders.end())
        m_loaders.push_back(loader);
}

mesh_ptr mesh_loader::load(const std::string & file)
{
    bool found_usable_loader=false;
    resource<mesh> res;
    res = this->get_resource(file);

    if(res.resource)
    {
        m_logger->log(LOG_LOG, "Found mesh in cache, skipping loading.");
        return res.resource;
    }

    std::string ext = file.substr(file.find_last_of('.'));
    m_logger->log(LOG_LOG, "Mesh extension: '%s'", ext.c_str());

    if(PHYSFS_exists(file.c_str()))
    for(imesh_loader * l : m_loaders)
    {
        if(l->check_by_extension(ext))
        {
            found_usable_loader = true;
            char * buf;
            uint32_t len = read(file,buf);

            if(len!=0)
            {
                m_logger->log(LOG_LOG, "Mesh file size: %u", len);

                res.path = file;
                res.resource = mesh_ptr(l->load(buf,len));
                this->add_resource(res);
                res.resource->init();
                return res.resource;
            }
            else
            {
                m_logger->log(LOG_ERROR, "File %s appears to be empty.",file.c_str());
            }
        }
    }

    if(!found_usable_loader)
        m_logger->log(LOG_ERROR, "No loader can load '%s' mesh files.",ext.c_str());

    return nullptr;
}
