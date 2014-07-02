#include "Precomp.h"

#define RELEASE_FS

#include "Application/Application.h"
#include "tests/GUIAndFontsApplication.h"
#include "tests/TinyXMLTestApplication.h"
#include "Applications/baigiamasis_quadcopter/BaigiamasisQuadcopter.h"
#include "Applications/game/TestGame.h"
#include "utility/Logger.h"
#include "utility/Timer.h"

int main(int argc, const char ** argv)
{
    Application * app = new baigiamasis_quadcopter(argc,argv);

    if(app->Init("Kursinis darbas",1280,720))
    {
        while(app->Update());

        app->Exit();
        delete app;
    }

    return 0;
}
