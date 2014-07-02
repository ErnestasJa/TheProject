#include "precomp.h"

#define RELEASE_FS

#include "application/application.h"
#include "tests/gui_and_fonts_application.h"
#include "tests/tinyxml_test_application.h"
#include "applications/kursinis/kursinis.h"
#include "applications/baigiamasis_quadcopter/baigiamasis_quadcopter.h"
#include "applications/game/test_game.h"
#include "utility/logger.h"
#include "utility/timer.h"

int main(int argc, const char ** argv)
{
    application * app = new kursinis(argc,argv);

    if(app->init("Kursinis darbas",1280,720))
    {
        while(app->update());

        app->exit();
        delete app;
    }

    return 0;
}
