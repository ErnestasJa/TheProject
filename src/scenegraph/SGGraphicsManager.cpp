#include "Precomp.h"
#include "SGGraphicsManager.h"
#include "opengl/Texture.h"
#include "utility/Logger.h"
#include "opengl/Mesh.h"
#include "scenegraph/SGMaterial.h"
#include "scenegraph/SGMeshObject.h"

namespace sg
{

sg_graphics_manager::sg_graphics_manager(Logger * l)
{
    _logger = l;
    m_image_loader = new image_loader(l);
    m_mesh_loader = new mesh_loader(l);
    m_shader_loader = new shader_loader(l);

    m_default_tex = load_texture("res/no_tex.png");
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
        _logger->log(LOG_LOG, "Found texture in cache, skipping loading.");
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
    {
        _logger->log(LOG_ERROR, "Texture '%s' could not be loaded.", file.c_str());
        //throw "Texture not loaded.";
    }
    else
        _logger->log(LOG_LOG, "Texture '%s' loaded.", file.c_str());


    return res.resource;
}

sg_material_ptr sg_graphics_manager::create_material(const std::string & vert_shader, const std::string & frag_shader, uint32_t type)
{
    sg_material * mat = new sg_material(m_shader_loader->load(vert_shader,frag_shader));

    if(!mat->mat_shader)
    {
        _logger->log(LOG_ERROR, "Could not load '%s' shader.", (vert_shader+" "+frag_shader).c_str());
        delete mat;
        return sg_material_ptr();
    }

    return share(mat);
}

resource_cache<texture> & sg_graphics_manager::get_texture_cache()
{
    return m_texture_cache;
}

image_loader * sg_graphics_manager::GetImageLoader()
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
