#ifndef SG_GRAPHICS_MANAGER_H
#define SG_GRAPHICS_MANAGER_H

#include "resources/ResourceCache.h"
#include "resources/ImageLoader.h"
#include "resources/MeshLoader.h"
#include "resources/ShaderLoader.h"


struct texture; typedef std::shared_ptr<texture> texture_ptr;

namespace sg{struct sg_material; typedef std::shared_ptr<sg_material> sg_material_ptr;}
class Logger;

namespace sg
{
class sg_mesh_object; struct sg_material; typedef std::shared_ptr<sg_mesh_object> sg_mesh_object_ptr;

class sg_graphics_manager
{
public:
    sg_graphics_manager(Logger * l);
    virtual ~sg_graphics_manager();

public:
    texture_ptr load_texture(std::string file);

    ///vert_shader, frag_shader should be optional for most materials.
    sg_material_ptr create_material(const std::string & vert_shader, const std::string & frag_shader, uint32_t id = 0);

public:
    resource_cache<texture> & get_texture_cache();

    image_loader * GetImageLoader();
    mesh_loader * get_mesh_loader();
    shader_loader * get_shader_loader();

protected:
    Logger          * _logger;
    image_loader    * m_image_loader;
    mesh_loader     * m_mesh_loader;
    shader_loader   * m_shader_loader;

    resource_cache<texture> m_texture_cache;

    texture_ptr       m_default_tex;
};

typedef std::shared_ptr<sg_graphics_manager> sg_graphics_manager_ptr;
}
#endif // GRAPHICS_MANAGER_H
