#pragma once

#include "iimage_loader.h"

class png_loader: public iimage_loader
{
    public:
        png_loader();
        virtual ~png_loader();

        virtual image * load(void * buffer, const uint32_t size);
        virtual bool check_by_extension(const std::string & ext);

    protected:
    private:
};
