#include "timer.h"
#include <chrono>
typedef std::chrono::duration<uint32_t,std::milli> milliseconds;

timer::timer()
{
    update_real_time();
    this->start_time = this->real_time;
    this->time = 0;
}

timer::timer(uint32_t begin_time)
{
    set_time(begin_time);
}

timer::~timer()
{

}

uint32_t timer::get_time()
{
    return time;
}

uint32_t timer::get_real_time()
{
    return real_time;
}

uint32_t timer::get_start_time()
{
    return start_time;
}

void timer::set_time(uint32_t time)
{
    update_real_time();
    this->start_time = this->real_time + time;
    this->time = time;
}

void timer::update_real_time()
{
    const std::chrono::high_resolution_clock::time_point & rt = std::chrono::high_resolution_clock::now();
    milliseconds ms = std::chrono::duration_cast<milliseconds>(rt.time_since_epoch());
    real_time = ms.count();
}

void timer::tick()
{
    update_real_time();
    time = real_time - start_time;
}

