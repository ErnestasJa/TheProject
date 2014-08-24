#include "Precomp.h"
#include "ApplicationLauncher.h"

ApplicationLauncher::ApplicationLauncher()
{
    m_currentApplication = nullptr;
}

ApplicationLauncher::~ApplicationLauncher()
{
    //dtor
}

void ApplicationLauncher::RegisterApplication(std::string applicationName, CreateAppFunc createFunc)
{
    m_creationFuncs.push_back(std::make_tuple(applicationName,createFunc));
}

uint32_t ApplicationLauncher::GetApplicationCount()
{
    return m_creationFuncs.size();
}

std::string ApplicationLauncher::GetApplicationName(uint32_t index)
{
    return std::get<0>(m_creationFuncs[index]);
}

void ApplicationLauncher::RunApplication(uint32_t index, int argc, const char ** argv)
{
    m_currentApplication = std::get<1>(m_creationFuncs[index])(argc,argv);
}
/*
void ApplicationLauncher::RunApplication(std::string name, int argc, const char ** argv)
{
    for(auto t : m_creationFuncs){
        if(name == std::get<0>(t))
            m_currentApplication = std::get<1>(t)();
    }
}*/

Application * ApplicationLauncher::GetCurrentApplication()
{
    return m_currentApplication;
}
