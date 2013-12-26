#pragma once

#include "image.h"

class iimage_loader
{
    public:
        iimage_loader();
        virtual ~iimage_loader();

        virtual image * load(void * buffer, const uint32_t size) = 0;
        virtual bool check_by_extension(const std::string & ext) = 0;

    protected:
    private:
};
