#include "precomp.h"
#include "sg_graphics_manager.h"
#include "opengl/texture.h"
#include "utility/logger.h"
#include "opengl/mesh.h"
#include "scenegraph/sg_material.h"
#include "scenegraph/sg_mesh_object.h"

namespace sg
{

sg_graphics_manager::sg_graphics_manager(logger * l)
{
    m_logger = l;
    m_image_loader = new image_loader(l);
    m_mesh_loader = new mesh_loader(l);
    m_shader_loader = new shader_loader(l);
}

sg_graphics_manager::~sg_graphics_manager()
{
    delete m_image_loader;
    delete m_mesh_loader;
    delete m_shader_loader;
}

texture_ptr sg_graphics_manager::load_texture(std::string file)
{
    resource<texture> res = m_texture_cache.get_resource(file);

    if(res.resource)
    {
        m_logger->log(LOG_LOG, "Found texture in cache, skipping loading.");
        return res.resource;
    }

    image_ptr img = m_image_loader->load(file);

    if(img)
    {
        res.path = file;
        res.resource = texture_ptr(new texture());
        res.resource->init(img);
        m_texture_cache.add_resource(res);
    }

    if(!res.resource)
        m_logger->log(LOG_ERROR, "Could not load '%s' texture.", file.c_str());

    return res.resource;
}

sg_material_ptr sg_graphics_manager::create_material(uint32_t type)
{
    switch(type)
    {
    case SGM_STATIC_MESH:
        {
            sg_material_static_mesh * mat = new sg_material_static_mesh();
            mat->mat_shader=m_shader_loader->load("res/static_mesh");

            if(!mat->mat_shader)
                 m_logger->log(LOG_ERROR, "Could not load '%s' shader.", "res/static_mesh");

            mat->mat_texture=load_texture("res/no_tex.png");
            return share(mat);
        }
    case SGM_POINT_SPRITE:
        {
            sg_material_point_sprite * mat = new sg_material_point_sprite();
            mat->mat_shader=m_shader_loader->load("res/shaders/point_sprite/point_sprite");
            mat->mat_texture=load_texture("res/no_tex.png");
            return share(mat);
        }
        default:
        return sg_material_ptr();
    }
}

resource_cache<texture> & sg_graphics_manager::get_texture_cache()
{
    return m_texture_cache;
}

image_loader * sg_graphics_manager::get_image_loader()
{
    return m_image_loader;
}

mesh_loader * sg_graphics_manager::get_mesh_loader()
{
    return m_mesh_loader;
}

shader_loader * sg_graphics_manager::get_shader_loader()
{
    return m_shader_loader;
}

} /// namespace sg
