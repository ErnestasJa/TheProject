#pragma once

#include "iimage_loader.h"
class logger;
class tgaloader: public iimage_loader
{
protected:

    #pragma pack(push)  /* push current alignment to stack */
    #pragma pack(1)     /* set alignment to 1 byte boundary */

    typedef struct {
        int8_t  idlength;
        int8_t  colourmaptype;
        int8_t  datatypecode;
        int16_t colourmaporigin;
        int16_t colourmaplength;
        int8_t  colourmapdepth;
        int16_t x_origin;
        int16_t y_origin;
        int16_t width;
        int16_t height;
        int8_t  bitsperpixel;
        int8_t  imagedescriptor;
    } HEADER;

    #pragma pack(pop)   /* restore original alignment from stack */

    virtual image * loadUncompressedTGA(void * buffer, const uint32_t size);

    HEADER m_header;

    logger * m_logger;

public:

    tgaloader(logger * l);
    virtual ~tgaloader();

    virtual image_ptr load(void * buffer, const uint32_t size);

    ///Check if the data is loadable
    virtual bool check_by_extension(const std::string & ext);

    virtual bool checkByHeader(void * hbuffer, const uint32_t hsize);

    ///Header size might be used to check file header
    virtual uint32_t getHeaderSize();

private:
};
