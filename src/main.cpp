#include "precomp.h"

#include "application/application.h"
#include "tests/test_application.h"
#include "tests/gui_and_fonts_application.h"
#include "tests/test_scenegraph.h"
#include "utility/logger.h"
#include "utility/timer.h"

int main(int argc, const char ** argv)
{
    application * app = new test_scenegraph(argc,argv);

    if(app->init("test",1024,720))
    {
        app->get_logger()->log(LOG_CRITICAL,"Testing %s %i %f.","vodka vodka boris yeltsin",666,3.14);
        while(app->update());

        app->exit();
        delete app;
    }

    std::string s;
    std::getline(std::cin,s);
    return 0;
}
