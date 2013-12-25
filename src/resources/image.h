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
};

#endif // IMAGE_H
