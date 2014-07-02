#include "Precomp.h"
#include "SGGraphicsManager.h"
#include "opengl/Texture.h"
#include "utility/Logger.h"
#include "opengl/Mesh.h"
#include "scenegraph/SGMaterial.h"
#include "scenegraph/SGMeshObject.h"

namespace sg
{

sg_graphics_manager::sg_graphics_manager(logger * l)
{
    m_logger = l;
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
        //throw "Texture not loaded.";
    }
    else
        m_logger->log(LOG_LOG, "Texture '%s' loaded.", file.c_str());


    return res.resource;
}

sg_material_ptr sg_graphics_manager::create_material(uint32_t type, const std::string & vert_shader, const std::string & frag_shader)
{
    switch(type)
    {
    case SGM_ABSTRACT_MATERIAL:
        {
            sg_abstract_material * mat = new sg_abstract_material(m_shader_loader->load(vert_shader,frag_shader));

            if(!mat->mat_shader)
            {
                 m_logger->log(LOG_ERROR, "Could not load '%s' shader.", (vert_shader+" "+frag_shader).c_str());
                 delete mat;
                 return sg_material_ptr();
            }

            return share(mat);
        }
    case SGM_STATIC_MESH:
        {
            sg_material_static_mesh * mat = new sg_material_static_mesh(m_shader_loader->load("res/shaders/static_mesh/static_mesh"));

            if(!mat->mat_shader)
            {
                 m_logger->log(LOG_ERROR, "Could not load '%s' shader.", "res/shaders/static_mesh/static_mesh");
                 delete mat;
                 return sg_material_ptr();
            }

            mat->mat_texture=m_default_tex;

            if(mat->mat_shader->bindings.size()==0)
            {
                mat->mat_shader->set();
                sg_mvar<int32_t>(mat->mat_shader->get_binding("texture0").index,"texture0",0).set();
            }

            return share(mat);
        }
    case SGM_VSM_FIRST_PASS:
        {
            sg_material_vsm_first_pass * mat = new sg_material_vsm_first_pass(m_shader_loader->load("res/shaders/vsm/firstStep"));

            if(!mat->mat_shader)
            {
                 m_logger->log(LOG_ERROR, "Could not load '%s' shader.", "res/shaders/vsm/firstStep");
                 delete mat;
                 return sg_material_ptr();
            }

            if(mat->mat_shader->bindings.size()==0)
            {
                mat->mat_shader->set();
            }

            return share(mat);
        }
    case SGM_VSM_FINAL_PASS:
        {
            sg_material_vsm_final_pass * mat = new sg_material_vsm_final_pass(m_shader_loader->load("res/shaders/vsm/VarianceShadowMapping"));


            if(!mat->mat_shader)
            {
                 m_logger->log(LOG_ERROR, "Could not load '%s' shader.", "res/shaders/vsm/VarianceShadowMapping");
                 delete mat;
                 return sg_material_ptr();
            }

            if(mat->mat_shader->bindings.size()==0)
            {
                mat->mat_shader->set();
                sg_mvar<int32_t>(mat->mat_shader->get_binding("texture0").index,"texture0",0).set();
            }


            mat->texture0=m_default_tex;
            mat->texture1=m_default_tex;


            return share(mat);
        }
    case SGM_TEXTURE_FILTER:
        {
            sg_material_texture_filter * mat = new sg_material_texture_filter(m_shader_loader->load(vert_shader,frag_shader));

            if(!mat->mat_shader)
            {
                 m_logger->log(LOG_ERROR, "Could not load '%s', '%s' shaders.", vert_shader.c_str(), frag_shader.c_str());
                 delete mat;
                 return sg_material_ptr();
            }

            if(mat->mat_shader->bindings.size()==0)
            {
                mat->mat_shader->set();
                sg_mvar<int32_t>(mat->mat_shader->get_binding("texture0").index,"texture0",0).set();
            }

            return share(mat);
        }
    case SGM_POINT_SPRITE:
        {
            sg_material_point_sprite * mat = new sg_material_point_sprite(m_shader_loader->load("res/shaders/point_sprite/point_sprite"));

            mat->mat_texture=m_default_tex;
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
