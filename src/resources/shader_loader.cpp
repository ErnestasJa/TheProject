#include "precomp.h"
#include "shader_loader.h"

shader_loader::shader_loader()
{
    //ctor
}

shader_loader::~shader_loader()
{
    //dtor
}

shader_ptr shader_loader::load(const std::string & file)
{
    resource<shader> res;

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

    return res.resource;
}
