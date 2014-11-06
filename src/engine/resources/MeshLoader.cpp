#include "Precomp.h"
#include "MeshLoader.h"
#include "IQMloader.h"
#include "opengl/Mesh.h"
#include "utility/Logger.h"

mesh_loader::mesh_loader(Logger * l)
{
    _logger = l;
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

MeshPtr mesh_loader::load(const std::string & file)
{
    bool found_usable_loader=false;
    resource<Mesh> res;
    res = this->get_resource(file);

    if(res.resource)
    {
        _logger->log(LOG_LOG, "Found mesh in cache, skipping loading.");
        return res.resource;
    }

    std::string ext = file.substr(file.find_last_of('.'));
    _logger->log(LOG_LOG, "Mesh extension: '%s'", ext.c_str());

    if(PHYSFS_exists(file.c_str()))
    for(imesh_loader * l : m_loaders)
    {
        if(l->check_by_extension(ext))
        {
            found_usable_loader = true;
            char * buf;
            uint32_t len = helpers::read(file,buf);

            if(len!=0)
            {
                _logger->log(LOG_LOG, "Mesh file size: %u", len);

                res.path = file;
                res.resource = MeshPtr(l->load(buf,len));
                this->add_resource(res);
                res.resource->Init();
                return res.resource;
            }
            else
            {
                _logger->log(LOG_ERROR, "File %s appears to be empty.",file.c_str());
            }
        }
    }

    if(!found_usable_loader)
        _logger->log(LOG_ERROR, "No loader can load '%s' mesh files.",ext.c_str());

    return nullptr;
}
