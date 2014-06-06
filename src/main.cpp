#include "precomp.h"

#define RELEASE_FS

#include "application/application.h"
#include "tests/gui_and_fonts_application.h"
#include "tests/tinyxml_test_application.h"
#include "tests/kursinis/test_kursinis.h"
#include "tests/game/baigiamasis_quadcopter.h"
#include "tests/game/test_game.h"
#include "utility/logger.h"
#include "utility/timer.h"

int main(int argc, const char ** argv)
{
    application * app = new test_kursinis(argc,argv);

    if(app->init("KOSS V0.1",1280,720))
    {
        while(app->update());

        app->exit();
        delete app;
    }

    return 0;
}
