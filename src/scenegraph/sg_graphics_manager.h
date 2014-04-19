#ifndef SG_GRAPHICS_MANAGER_H
#define SG_GRAPHICS_MANAGER_H

#include "resources/resource_cache.h"
#include "resources/image_loader.h"
#include "resources/mesh_loader.h"
#include "resources/shader_loader.h"


struct texture; typedef std::shared_ptr<texture> texture_ptr;

namespace sg{struct sg_material; typedef std::shared_ptr<sg_material> sg_material_ptr;}
class logger;

namespace sg
{
class sg_mesh_object; struct sg_material; typedef std::shared_ptr<sg_mesh_object> sg_mesh_object_ptr;

class sg_graphics_manager
{
public:
    sg_graphics_manager(logger * l);
    virtual ~sg_graphics_manager();

public:
    texture_ptr load_texture(std::string file);

    ///vert_shader, frag_shader should be optional for most materials.
    sg_material_ptr create_material(uint32_t type, const std::string & vert_shader = "", const std::string & frag_shader = "");

public:
    resource_cache<texture> & get_texture_cache();

    image_loader * get_image_loader();
    mesh_loader * get_mesh_loader();
    shader_loader * get_shader_loader();

protected:
    logger          * m_logger;
    image_loader    * m_image_loader;
    mesh_loader     * m_mesh_loader;
    shader_loader   * m_shader_loader;

    resource_cache<texture> m_texture_cache;

    texture_ptr       m_default_tex;
};

typedef std::shared_ptr<sg_graphics_manager> sg_graphics_manager_ptr;
}
#endif // GRAPHICS_MANAGER_H
