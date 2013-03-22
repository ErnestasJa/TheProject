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

    if((m_header.width <= 0) || (m_header.height <= 0) || ((m_header.bitsperpixel != 24) && (m_header.bitsperpixel !=32)))
    {
        printf("Somethings wrong: %i, %i, %i\n",(int)m_header.width ,(int)m_header.height ,(int)m_header.bitsperpixel );
        return 0;
    }

    uint8_t * data = new uint8_t[m_header.width*m_header.height*m_header.bitsperpixel/8];

    memcpy(data,&static_cast<uint8_t*>(buffer)[this->getHeaderSize()],m_header.width*m_header.height*m_header.bitsperpixel/8);

    uint8_t tmp;

    for(uint32_t i = 0; i < (uint32_t)m_header.width*m_header.height*m_header.bitsperpixel/8; i += m_header.bitsperpixel/8)
    {
        tmp=data[i];
        data[i]=data[i+2];
        data[i+2]=tmp;
    }

    texture * tex = new texture();

    tex->generate(data, GL_TEXTURE_2D, m_header.bitsperpixel/8, m_header.width,m_header.height);

    return tex;
}

bool tgaloader::checkByExtension(const std::string & ext)
{
    if(ext=="tga")
        return true;

    return false;
}

bool tgaloader::checkByHeader(void * hbuffer, const uint32_t hsize)
{
    return true;
}

uint32_t tgaloader::getHeaderSize()
{
    return sizeof(HEADER);
}
