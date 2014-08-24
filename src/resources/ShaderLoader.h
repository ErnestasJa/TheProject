#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "opengl/Shader.h"
#include "ResourceCache.h"

class Logger;
class shader_loader: public resource_cache<Shader>
{
    public:
        shader_loader(Logger * l);
        virtual ~shader_loader();

        ShaderPtr load(const std::string & file);
        ShaderPtr load(const std::string & vertex_file, const std::string & fragment_file);
        ShaderPtr get_shader_by_name(const std::string & name);
    protected:
        Logger * _logger;
};

#endif // SHADER_LOADER_H
