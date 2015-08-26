#pragma once

//logger levels
enum loglevel
{
    LOG_LOG=0, //just logging
    LOG_DEBUG, //output for debugging
    LOG_WARN, //warnings
    LOG_ERROR, //errors
    LOG_CRITICAL //we're fucked
};

class Application;
class PHYSFS_File;
class Logger
{
public:
    Logger(Application *app, int verbosity); //to be decided
    //verbosity 0: all output
    //verbosity 1: only warnings and errors
    //verbosity 2: only errors
    ~Logger();

    void log(loglevel lev,const char* st, ...);
private:
    int m_verbosity;
    Application * m_app;
    PHYSFS_File * m_logfile;
    //vector< std::pair<debuglevel,std::string> > outputs; //wrapped outputs for easy output

    std::string timestamp(); //timestamp generation
};
