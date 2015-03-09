#include "Precomp.h"

#include "application/Application.h"
#include "application/ApplicationLauncher.h"
#include "utility/Logger.h"
#include "utility/Timer.h"

#include "VoxelzApp.h"
#include "VoxelzProfilingApp.h"
#include "VoxMeshEditorApp.h"
#include "CollisionTestApp.h"

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
    appLauncher.RegisterApplication("VoxelZ application", APP(VoxelzApp));
    appLauncher.RegisterApplication("VoxMeshEditor application", APP(VoxMeshEditorApp));
    appLauncher.RegisterApplication("VoxelZ profiling application", APP(VoxelzProfilingApp));
    appLauncher.RegisterApplication("Collision test application", APP(CollisionTestApp));

    char choice = ShowAppMenu(appLauncher);
    uint32_t nr =  choice - '0';

    while(choice != '\n' && nr > 0 && nr <= appLauncher.GetApplicationCount() )
    {
        appLauncher.RunApplication(nr-1,argc,argv);

        if(appLauncher.GetCurrentApplication()->Init(appLauncher.GetApplicationName(nr-1),1280,768))
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
