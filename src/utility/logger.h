#pragma once

template <typename T> static std::string tostr(const T& t) { std::ostringstream os; os<<t; return os.str(); } //magic..

//logger levels
enum loglevel
{
    LOG_LOG=0, //just logging
    LOG_DEBUG, //output for debugging
    LOG_WARN, //warnings
    LOG_ERROR, //errors
    LOG_CRITICAL //we're fucked
};


class application;
class PHYSFS_File;

class logger
{
public:
    logger(application *app, int verbosity); //to be decided
    //verbosity 0: all output
    //verbosity 1: only warnings and errors
    //verbosity 2: only errors
    ~logger();

    void log(loglevel lev,const char* st, ...);
private:
    int m_verbosity;
    application * m_app;
    PHYSFS_File * m_logfile;
    //std::vector< std::pair<debuglevel,std::string> > outputs; //wrapped outputs for easy output

    std::string timestamp(); //timestamp generation
};
