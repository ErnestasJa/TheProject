#include "precomp.h"
#include "logger.h"

applogger logger(0);

applogger::applogger(int verbosity)
{
    _verbosity=verbosity;
    _time=time(0);
    log(LOG_DEBUG,"Logger started...");
}

applogger::~applogger()
{
    log(LOG_DEBUG,"Logger is terminating...");
}

void applogger::log(loglevel lev,const char* st, ...)
{
    char buf[256];
    va_list l;
    va_start(l,st);
    vsnprintf(buf,256,st,l);
    va_end(l);
    std::string message="";

    message+="["+timestamp()+"] ";
    //add importance info
    switch(lev)
    {
    case LOG_LOG:
        message+="LOG: ";
        break;
    case LOG_DEBUG:
        message+="DEBUG: ";
        break;
    case LOG_WARN:
        message+="WARNING: ";
        break;
    case LOG_ERROR:
        message+="ERROR: ";
        break;
    case LOG_CRITICAL:
        message+="CRITICAL: ";
        break;
    default:
        break;
    }

    message.append(buf);
    //std::pair<debuglevel,std::string> p(lev,message);
    //outputs.push_back(p);
    printf("%s\n",message.c_str());
}

std::string applogger::timestamp()
{
    std::string stamp="";

    uint32_t t=(uint32_t)difftime(time(0),_time);

    uint32_t h,m,s;

    h=t/3600;
    m=t/60;
    s=t-h*3600-m*60;

    if(h<10)
        stamp+="0";
    stamp+=tostr(h);
    stamp+=":";
    if(m<10)
        stamp+="0";
    stamp+=tostr(m);
    stamp+=":";
    if(s<10)
        stamp+="0";
    stamp+=tostr(s);

    return stamp;
}
