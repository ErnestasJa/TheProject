#include "Precomp.h"

#include "TGAloader.h"
#include "glm.hpp"

#include "utility/Logger.h"

tgaloader::tgaloader(Logger * l): _logger(l)
{

}

tgaloader::~tgaloader()
{
    //dtor
}

image_ptr tgaloader::load(void * buffer, const uint32_t size)
{
    memcpy(&m_header, buffer, this->getHeaderSize());

    if(m_header.datatypecode==2)/// load the uncompressed TGA
        return image_ptr(this->loadUncompressedTGA(buffer,size));
    else
        printf("bad tga header: %i\n",(int)m_header.datatypecode);

    return image_ptr(NULL);
}

image * tgaloader::loadUncompressedTGA(void * buffer, const uint32_t size)
{
    //memcpy(&tga.header, buffer, this->getHeaderSize());
    uint8_t components=m_header.bitsperpixel/8;
    if(m_header.width <= 0 || m_header.height <= 0 || (components != 3 && components != 4))
    {
        printf("Somethings wrong: %i, %i, %i\n",(int)m_header.width ,(int)m_header.height ,(int)m_header.bitsperpixel );
        return 0;
    }

    uint8_t * data = new uint8_t[ m_header.width * m_header.height * components ];

    memcpy(data,&static_cast<uint8_t*>(buffer)[this->getHeaderSize()],m_header.width*m_header.height*components);

    uint8_t tmp;

    for(uint32_t i = 0; i < (uint32_t)m_header.width*m_header.height*components; i += components)
    {
        tmp=data[i];
        data[i]=data[i+2];
        data[i+2]=tmp;
    }

    if(data)
    {
        image * r = new image();
        r->data = data;
        r->num_channels = components;
        r->width = m_header.width;
        r->height = m_header.height;
        return r;
    }

    return nullptr;
}

bool tgaloader::check_by_extension(const std::string & ext)
{
    return ext=="tga" || ext==".tga";
}

bool tgaloader::checkByHeader(void * hbuffer, const uint32_t hsize)
{
    return true;
}

uint32_t tgaloader::getHeaderSize()
{
    return sizeof(HEADER);
}
