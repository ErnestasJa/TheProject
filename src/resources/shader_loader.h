#ifndef SHADER_LOADER_H
#define SHADER_LOADER_H

#include "opengl/shader.h"
#include "resource_cache.h"
class shader_loader: public resource_cache<shader>
{
    public:
        shader_loader();
        virtual ~shader_loader();

        shader_ptr load(const std::string & file);
};

#endif // SHADER_LOADER_H
