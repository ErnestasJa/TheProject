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
    {
        m_logger->log(LOG_ERROR, "Texture '%s' could not be loaded.", file.c_str());
        throw "Texture not loaded.";
    }
    else
        m_logger->log(LOG_LOG, "Texture '%s' loaded.", file.c_str());


    return res.resource;
}

sg_material_ptr sg_graphics_manager::create_material(uint32_t type, const std::string & vert_shader, const std::string & frag_shader)
{
    switch(type)
    {
    case SGM_STATIC_MESH:
        {
            sg_material_static_mesh * mat = new sg_material_static_mesh();
            mat->mat_shader=m_shader_loader->load("res/shaders/static_mesh/static_mesh");

            if(!mat->mat_shader)
            {
                 m_logger->log(LOG_ERROR, "Could not load '%s' shader.", "res/static_mesh/static_mesh");
                 delete mat;
                 return sg_material_ptr();
            }

            mat->mat_texture=load_texture("res/no_tex.png");

            if(sg_material_static_mesh::bindings[0].index==-1)
            {
                mat->mat_shader->set();
                mat->mat_shader->query_binding_locations(sg_material_static_mesh::bindings);
                sg_material_static_mesh::init_bindings();
            }

            return share(mat);
        }
    case SGM_VSM_FIRST_PASS:
        {
            sg_material_vsm_first_pass * mat = new sg_material_vsm_first_pass();
            mat->mat_shader=m_shader_loader->load("res/shaders/vsm/firstStep");

            if(!mat->mat_shader)
            {
                 m_logger->log(LOG_ERROR, "Could not load '%s' shader.", "res/shaders/vsm/firstStep");
                 delete mat;
                 return sg_material_ptr();
            }

            if(sg_material_vsm_first_pass::bindings[0].index==-1)
            {
                mat->mat_shader->set();
                mat->mat_shader->query_binding_locations(sg_material_vsm_first_pass::bindings);
            }

            return share(mat);
        }
    case SGM_VSM_FINAL_PASS:
        {
            sg_material_vsm_final_pass * mat = new sg_material_vsm_final_pass();
            mat->mat_shader=m_shader_loader->load("res/shaders/vsm/VarianceShadowMapping");

            if(!mat->mat_shader)
            {
                 m_logger->log(LOG_ERROR, "Could not load '%s' shader.", "res/shaders/vsm/VarianceShadowMapping");
                 delete mat;
                 return sg_material_ptr();
            }

            //mat->texture0=load_texture("res/no_tex.png");
            mat->texture1=load_texture("res/no_tex.png");

            if(sg_material_vsm_final_pass::bindings[0].index==-1)
            {
                mat->mat_shader->set();
                mat->mat_shader->query_binding_locations(sg_material_vsm_final_pass::bindings);
                sg_material_vsm_final_pass::init_bindings();
            }



            return share(mat);
        }
    case SGM_TEXTURE_FILTER:
        {
            sg_material_texture_filter * mat = new sg_material_texture_filter();
            mat->mat_shader=m_shader_loader->load(vert_shader,frag_shader);

            if(!mat->mat_shader)
            {
                 m_logger->log(LOG_ERROR, "Could not load '%s', '%s' shaders.", vert_shader.c_str(), frag_shader.c_str());
                 delete mat;
                 return sg_material_ptr();
            }

            //mat->texture0=load_texture("res/no_tex.png");

            if(sg_material_texture_filter::bindings[0].index==-1)
            {
                mat->mat_shader->set();
                mat->mat_shader->query_binding_locations(sg_material_texture_filter::bindings);
            }

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
