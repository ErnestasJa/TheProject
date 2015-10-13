#include "Precomp.h"

#define RELEASE_FS

#include "application/Application.h"
#include "application/ApplicationLauncher.h"
#include "VoxelOctreeApp.h"
#include "VoxelOctreeBenchmarkApp.h"
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

void StartApp(uint32_t app, ApplicationLauncher & launcher, int argc, const char ** argv)
{
    launcher.RunApplication(app-1,argc,argv);

    if(launcher.GetCurrentApplication()->Init(launcher.GetApplicationName(app-1)))
      {
	while(launcher.GetCurrentApplication()->Update());

	launcher.GetCurrentApplication()->Exit();
	delete launcher.GetCurrentApplication();
      }
}


int main(int argc, const char ** argv)
{
    ApplicationLauncher appLauncher;
    appLauncher.RegisterApplication("Voxel octree application", APP(VoxelOctreeApp));
    appLauncher.RegisterApplication("Voxel octree benchmark application", APP(VoxelOctreeBenchmarkApp));

    int appToLaunch = 0;
    char choice;
    uint32_t nr;
    
    if(argc>=2)
      nr = (int) argv[1][0] - '0';

    if(nr != 0){
      std::cout << "Starting app: " << nr << std::endl;  
      StartApp(nr, appLauncher, argc, argv);
    }

    return 0;
}
