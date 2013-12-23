#include "tgaloader.h"
#include "glm.hpp"
#include <string.h>


tgaloader::tgaloader()
{

}

tgaloader::~tgaloader()
{
    //dtor
}

texture * tgaloader::generate(void * buffer, const uint32_t size)
{
    memcpy(&m_header, buffer, this->getHeaderSize());

    if(m_header.datatypecode==2)/// load the uncompressed TGA
        return this->loadUncompressedTGA(buffer,size);
    else
        printf("bad tga header: %i\n",(int)m_header.datatypecode);

    return NULL;
}

texture * tgaloader::loadUncompressedTGA(void * buffer, const uint32_t size)
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

    texture * tex = new texture();

    tex->generate(data, GL_TEXTURE_2D, m_header.bitsperpixel==32?GL_RGBA:GL_RGB, GL_RGBA, m_header.width,m_header.height);

    return tex;
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
