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

    if(!read(file + ".vert",vsh)) return shader_ptr();
    if(!read(file + ".frag",fsh)) return shader_ptr();

    shader * sh = new shader(file,vsh,fsh,default_texture_binding,0);
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
