#include "precomp.h"

#define RELEASE_FS

#include "Application/Application.h"
#include "tests/gui_and_fonts_Application.h"
#include "tests/tinyxml_test_Application.h"
#include "Applications/baigiamasis_quadcopter/baigiamasis_quadcopter.h"
#include "Applications/game/test_game.h"
#include "utility/logger.h"
#include "utility/timer.h"

int main(int argc, const char ** argv)
{
    Application * app = new baigiamasis_quadcopter(argc,argv);

    if(app->init("Kursinis darbas",1280,720))
    {
        while(app->update());

        app->exit();
        delete app;
    }

    return 0;
}
