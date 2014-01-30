#include "precomp.h"
#include "graphics_manager.h"
#include "texture.h"
#include "utility/logger.h"
#include "opengl/mesh.h"
#include "scenegraph/sg_material.h"
#include "scenegraph/sg_mesh_object.h"

graphics_manager::graphics_manager(logger * l)
{
    m_logger = l;
    m_image_loader = new image_loader(l);
    m_mesh_loader = new mesh_loader(l);
    m_shader_loader = new shader_loader(l);
}

graphics_manager::~graphics_manager()
{
    delete m_image_loader;
    delete m_mesh_loader;
    delete m_shader_loader;
}

texture_ptr graphics_manager::load_texture(std::string file)
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

    return res.resource;
}

sg::sg_mesh_object_ptr graphics_manager::load_mesh_object(std::string file, sg::sg_material default_mat, bool load_textures)
{
    sg::sg_mesh_object_ptr ret;
    mesh_ptr pmesh;

    pmesh = m_mesh_loader->load(file);

    if(pmesh)
    {
        pmesh->init();
        ret = share(new sg::sg_mesh_object(pmesh));

        loopi(ret->get_material_count())
        {
            sg::sg_material & mat = ret->get_material(i);
            mat.mat_shader=default_mat.mat_shader;

            std::string mat_name, image_path;
            uint32_t pos = pmesh->sub_meshes[i].material_name.find("|");
            mat_name = pmesh->sub_meshes[i].material_name.substr(0,pos-1);
            image_path = pmesh->sub_meshes[i].material_name.substr(pos+1);

            m_logger->log(LOG_LOG, "pos =%u", pos);
            m_logger->log(LOG_LOG, "mat_name =%s", mat_name.c_str());
            m_logger->log(LOG_LOG, "image_path =%s", image_path.c_str());

            if(load_textures)
                mat.mat_textures[0] = load_texture("res/maps/cs_assault/" + image_path);
        }
    }

    return ret;
}

resource_cache<texture> & graphics_manager::get_texture_cache()
{
    return m_texture_cache;
}

image_loader * graphics_manager::get_image_loader()
{
    return m_image_loader;
}

mesh_loader * graphics_manager::get_mesh_loader()
{
    return m_mesh_loader;
}

shader_loader * graphics_manager::get_shader_loader()
{
    return m_shader_loader;
}
