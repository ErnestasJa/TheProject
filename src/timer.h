#pragma once
#include "precomp.h"

class timer
{
protected:
    uint32_t start_time;
    uint32_t time;
    uint32_t real_time;

    void update_real_time();

public:

    timer();
    timer(uint32_t begin_time);
    virtual ~timer();

    uint32_t get_time();
    uint32_t get_start_time();
    void set_time(uint32_t time);

    uint32_t get_real_time();

    void tick();
};
