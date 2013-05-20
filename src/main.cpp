#include "application.h"
#include "test_application.h"

int main(int argc, const char ** argv)
{
    application * app = new test_application(argc,argv);
    if(app->init("test",1024,720))
    {
        app->getLogger()->log(LOG_LOG,"Testing %s %i %f.","vodka vodka boris yeltsin",666,3.14);
        while(app->update());

        app->exit();
        delete app;
    }
    else
        std::cout<<"Failed to initialize application." << std::endl;


    return 0;
}
