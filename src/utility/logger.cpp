#include "Precomp.h"

#include "utility/Logger.h"
#include "Application/Application.h"
#include "utility/Timer.h"

Logger::Logger(Application *app,int verbosity)
{
    #ifdef LOG_OUTPUT
    m_verbosity=verbosity;
    m_app=app;

    //set up filewriting
    PHYSFS_setWriteDir(PHYSFS_getBaseDir());
    //in case logs dir is not present
    PHYSFS_mkdir("logs");

    //format the filename with realtime stamp
    std::string fname="/logs/";
    fname+=helpers::to_str(m_app->GetTimer()->get_real_time());
    fname+="_log.txt";

    //open it
    m_logfile=PHYSFS_openWrite(fname.c_str());

    log(LOG_DEBUG,"Logger initialised...");
    #else
    printf("Logging is disabled.\n");
    #endif
}

Logger::~Logger()
{
    log(LOG_DEBUG,"Logger is terminating...");
    PHYSFS_close(m_logfile);
}

void Logger::log(loglevel lev,const char* st, ...)
{
    #ifdef LOG_OUTPUT
    m_app->GetTimer()->tick();
    char buf[256];
    va_list l;
    va_start(l,st);
    vsnprintf(buf,256,st,l);
    va_end(l);
    std::string message="";

    //message+="["+timestamp()+"] ";
    message+=helpers::to_str(m_app->GetTimer()->get_time())+" ";
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
    message.append("\n");
//    //std::pair<debuglevel,std::string> p(lev,message);
//    //outputs.push_back(p);
//    PHYSFS_write(m_logfile,message.c_str(),message.size(),1);
//    PHYSFS_flush(m_logfile);
    printf("%s",message.c_str());
    #endif // LOG_OUTPUT
}

std::string Logger::timestamp()
{
    std::string stamp="";

    uint32_t t=m_app->GetTimer()->get_time()/1000;

    uint32_t h,m,s;

    h=t/3600;
    m=t/60;
    s=t-h*3600-m*60;

    if(h<10)
        stamp+="0";
    stamp+=helpers::to_str(h);
    stamp+=":";
    if(m<10)
        stamp+="0";
    stamp+=helpers::to_str(m);
    stamp+=":";
    if(s<10)
        stamp+="0";
    stamp+=helpers::to_str(s);

    return stamp;
}
