#include "application.h"

#include "window.h"
#include "opengl_util.h"
#include "timer.h"

static void output_versions(void)
{
    PHYSFS_Version compiled;
    PHYSFS_Version linked;

    PHYSFS_VERSION(&compiled);
    PHYSFS_getLinkedVersion(&linked);

    printf("Compiled against PhysicsFS version %d.%d.%d,\n"
           " and linked against %d.%d.%d.\n\n",
            (int) compiled.major, (int) compiled.minor, (int) compiled.patch,
            (int) linked.major, (int) linked.minor, (int) linked.patch);
} /* output_versions */

application::application(int32_t argc, const char ** argv)
{
    this->wnd = nullptr;
    this->gl_util = nullptr;
    this->main_timer = nullptr;
    this->argc = argc;
    this->argv = argv;
}

application::~application()
{

}

bool application::init(const std::string  &title, uint32_t width, uint32_t height)
{
    std::cout << "Initializing \"" << title.c_str() << "\"" << std::endl;

    this->main_timer = new timer();

    if (!PHYSFS_init(argv[0]))
    {
        ///!Log this
        std::cout<<"PHYSFS_init() failed: " <<PHYSFS_getLastError()<<std::endl;
        return false;
    }

    PHYSFS_mount(PHYSFS_getBaseDir(), NULL, 0);

    output_versions();

    this->wnd = new window();

    if(!this->wnd->init(title, width, height))
    {
        delete wnd;
        return false;
    }

    this->gl_util = new opengl_util();

    if(!this->gl_util->load_extensions())
    {
        delete wnd;
        delete gl_util;
        return false;
    }

    return true;
}

void application::exit()
{
    std::cout << "Exiting." << std::endl;

    delete gl_util;
    delete wnd;

    if (!PHYSFS_deinit())
        std::cout << "PHYSFS_deinit() failed!\n  reason: " << PHYSFS_getLastError() << "." << std::endl;

    delete main_timer;
}

timer * application::get_timer()
{
    return main_timer;
}
