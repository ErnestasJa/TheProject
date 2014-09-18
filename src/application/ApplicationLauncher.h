#ifndef APPLICATIONLAUNCHER_H
#define APPLICATIONLAUNCHER_H


#include <string>
#include <tuple>
#include <stdint.h>

#include "Application.h"


typedef Application * (*CreateAppFunc)(int argc, const char ** argv);

class ApplicationLauncher
{
    public:
        ApplicationLauncher();
        virtual ~ApplicationLauncher();

        virtual void RegisterApplication(std::string applicationName, CreateAppFunc createFunc);
        virtual uint32_t GetApplicationCount();
        virtual std::string GetApplicationName(uint32_t index);
        virtual void RunApplication(uint32_t index, int argc, const char ** argv);
        virtual Application * GetCurrentApplication();
        //virtual void RunApplication(std::string name, int argc, const char ** argv);

    protected:
        vector<std::tuple<std::string, CreateAppFunc> > m_creationFuncs;
        Application * m_currentApplication;
    private:
};

#endif // APPLICATIONLAUNCHER_H
