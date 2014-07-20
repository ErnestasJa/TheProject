#include "Precomp.h"

#define RELEASE_FS

#include "Application/Application.h"
#include "applications/material_test/MaterialTest.h"
#include "utility/Logger.h"
#include "utility/Timer.h"

int main(int argc, const char ** argv)
{
    Application * app = new MaterialTest(argc,argv);

    if(app->Init("ZGP Application v0.01 pre alpha beta gama banana.",1280,720))
    {
        while(app->Update());

        app->Exit();
        delete app;
    }

    return 0;
}
