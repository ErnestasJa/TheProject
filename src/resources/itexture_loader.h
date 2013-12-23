#ifndef ITEXTURE_LOADER_H
#define ITEXTURE_LOADER_H

#include "opengl/texture.h"

class itexture_loader
{
    public:
        itexture_loader();
        virtual ~itexture_loader();

        virtual texture * generate(void * buffer, const uint32_t size) = 0;
        virtual bool check_by_extension(const std::string & ext) = 0;

    protected:
    private:
};

#endif // ITEXTURE_LOADER_H
