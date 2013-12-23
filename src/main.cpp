#include "application.h"
#include "test_application.h"
#include "gui_and_fonts_application.h"
#include "logger.h"
#include "timer.h"

int main(int argc, const char ** argv)
{
    application * app = new gui_and_fonts_application(argc,argv);

    if(app->init("test",1024,720))
    {
        app->get_logger()->log(LOG_CRITICAL,"Testing %s %i %f.","vodka vodka boris yeltsin",666,3.14);
        while(app->update());

        app->exit();
        delete app;
    }

    return 0;
}
