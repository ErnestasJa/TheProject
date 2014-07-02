#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "IMeshLoader.h"
#include "ResourceCache.h"

class logger;
class mesh_loader: public resource_cache<mesh>
{
public:
    mesh_loader(logger * l);
    virtual ~mesh_loader();

    virtual void add_loader(imesh_loader * loader);
    virtual mesh_ptr load(const std::string & file);

protected:
    logger * m_logger;
    std::vector<imesh_loader*> m_loaders;
};

#endif // MESH_LOADER_H
