#pragma once

#include "itexture_loader.h"

class png_loader: public itexture_loader
{
    public:
        png_loader();
        virtual ~png_loader();

        virtual texture * generate(void * buffer, const uint32_t size);
        virtual bool check_by_extension(const std::string & ext);

    protected:
    private:
};
