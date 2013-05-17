#pragma once

#include <stdio.h>
#include <string>
#include <sstream>
#include <time.h>
#include <stdarg.h>

template <typename T> static std::string tostr(const T& t) { std::ostringstream os; os<<t; return os.str(); } //magic..

enum loglevel
{
    LOG_LOG=0, //just logging
    LOG_DEBUG, //output for debugging
    LOG_WARN, //warnings
    LOG_ERROR, //errors
    LOG_CRITICAL //we're fucked
};

class applogger
{
public:
    applogger(int verbosity); //to be decided
    //verbosity 0: all output
    //verbosity 1: only warnings and errors
    //verbosity 2: only errors
    ~applogger();

    void log(loglevel lev,const char* st, ...);
private:
    time_t _time;
    int _verbosity;
    //std::vector< std::pair<debuglevel,std::string> > outputs; //wrapped outputs for easy output

    std::string timestamp(); //timestamp generation
};

extern applogger logger; //make it global
