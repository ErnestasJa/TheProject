#include "Precomp.h"

#define RELEASE_FS

#include "Application/Application.h"
#include "Application/ApplicationLauncher.h"
#include "applications/material_test/MaterialTest.h"
#include "utility/Logger.h"
#include "utility/Timer.h"

#define APP(AppName) [](int argc, const char ** argv)->Application*{ return new AppName(argc,argv);}

char ShowAppMenu(ApplicationLauncher & launcher)
{
    std::cout << std::endl << std::endl;
    std::cout<< "=================================" << std::endl;
    std::cout<< "Registered applications" << std::endl;
    std::cout<< "=================================" << std::endl;
    std::cout<< "[0]\t" << "Close application." << std::endl;

    for(uint32_t i = 0; i < launcher.GetApplicationCount(); i++)
    {
        std::cout<< "[" << i + 1 << "]\t" << launcher.GetApplicationName(i) << std::endl;
    }

    std::cout<< std::endl << "Which application would you like to run: " << std::endl;

    char choice = 0;
    choice = std::cin.get();
    std::cin.clear(); std::cin.ignore(INT_MAX,'\n');
    return choice;
}

int main(int argc, const char ** argv)
{
    ApplicationLauncher appLauncher;
    appLauncher.RegisterApplication("Material test", APP(MaterialTest));

    char choice = ShowAppMenu(appLauncher);
    int nr =  choice - '0';

    while(choice != '\n' && nr > 0 && nr <= appLauncher.GetApplicationCount() )
    {
        appLauncher.RunApplication(nr-1,argc,argv);

        if(appLauncher.GetCurrentApplication()->Init("ZGP Application v0.01 pre alpha beta gama banana.",1280,720))
        {
            while(appLauncher.GetCurrentApplication()->Update());

            appLauncher.GetCurrentApplication()->Exit();
            delete appLauncher.GetCurrentApplication();
        }

        choice = ShowAppMenu(appLauncher);
        nr =  choice - '0';
    }

    return 0;
}
