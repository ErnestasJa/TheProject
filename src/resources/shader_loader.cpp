#include "precomp.h"
#include "shader_loader.h"
#include "utility/logger.h"

shader_loader::shader_loader(logger * l)
{
    m_logger = l;
}

shader_loader::~shader_loader()
{
    //dtor
}

shader_ptr shader_loader::load(const std::string & file)
{
    resource<shader> res;

    res = this->get_resource(file);

    if(res.resource)
    {
        m_logger->log(LOG_LOG, "Found shader in cache, skipping loading.");
        return res.resource;
    }

    char * vsh=NULL;
    char * fsh=NULL;

    static binding default_texture_binding[] = {{"tex",0}, {"",-1}};

    if(!helpers::read(file + ".vert",vsh)) return shader_ptr();
    if(!helpers::read(file + ".frag",fsh)) return shader_ptr();

    std::string sh_name = file.substr(file.rfind("/")+1);
    m_logger->log(LOG_LOG, "Shader name: %s", sh_name.c_str());
    shader * sh = new shader(sh_name,vsh,fsh,default_texture_binding,0);
	sh->compile();
	sh->link();

	if(sh->program)
    {
        res.resource = shader_ptr(sh);
        res.path = file;
        this->add_resource(res);
    }

	delete [] vsh;
	delete [] fsh;

	if(res.resource)
        m_logger->log(LOG_LOG, "Shader '%s' loaded.",file.c_str());

    return res.resource;
}

shader_ptr shader_loader::get_shader_by_name(const std::string & name)
{
    for(resource<shader> & res: m_resources)
    {
        if(res.resource->name == name)
            return res.resource;
    }

    return shader_ptr();
}