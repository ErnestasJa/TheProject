#ifndef IMAGE_H
#define IMAGE_H

struct image
{
    uint32_t width, height, num_channels;
    uint8_t * data;

    image()
    {
        data = nullptr;
        width = 0, height=0, num_channels=0;
    }

    ~image()
    {
        if(data)
            delete [] data;

    }

    void Init(uint32_t width,uint32_t height,uint8_t channels)
    {
        this->width=width;
        this->height=height;
        this->num_channels=channels;

        data=new uint8_t[width*height*channels];
        loop(i,width)
        loop(j,height)
        {
            data[j*width*num_channels+i*num_channels]=0;
            data[j*width*num_channels+i*num_channels+1]=0;
            data[j*width*num_channels+i*num_channels+2]=0;
            if(num_channels==4) data[j*width*num_channels+i*num_channels+3]=0;
        }
    }

    void SetPixel(uint32_t x, uint32_t y,uint8_t r,uint8_t g,uint8_t b,uint8_t a=255)
    {
        if(x>width||y>height) return;

        data[y*width*num_channels+x*num_channels]=r;
        data[y*width*num_channels+x*num_channels+1]=g;
        data[y*width*num_channels+x*num_channels+2]=b;
        if(num_channels==4) data[y*width*num_channels+x*num_channels+3]=a;
    }

    glm::detail::tvec4<uint8_t> GetPixel(uint32_t x, uint32_t y)
    {
        if(x>width||y>height) return glm::detail::tvec4<uint8_t>(0,0,0,0);

        uint8_t r=data[y*width*num_channels+x*num_channels];
        uint8_t g=data[y*width*num_channels+x*num_channels+1];
        uint8_t b=data[y*width*num_channels+x*num_channels+2];
        uint8_t a=255;
        if(num_channels==4) a=data[y*width*num_channels+x*num_channels+3];

        return glm::detail::tvec4<uint8_t>(r,g,b,a);
    }
};

typedef std::shared_ptr<image> image_ptr;

#endif // IMAGE_H
