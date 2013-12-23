#include "application.h"

#include "window.h"
#include "opengl_util.h"
#include "timer.h"
#include "logger.h"

void application::output_versions()
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
    this->main_timer = new timer();
    if (!PHYSFS_init(argv[0]))
    {
        std::cout<<"PHYSFS_init() failed: " <<PHYSFS_getLastError()<<std::endl;
        return false;
    }

    std::string real_dir = PHYSFS_getBaseDir();

    std::string dir;

    uint32_t pos = real_dir.find_last_of(PHYSFS_getDirSeparator());
    real_dir = real_dir.substr(0,pos);

    pos = real_dir.find_last_of(PHYSFS_getDirSeparator());
    real_dir = real_dir.substr(0,pos);

    pos = real_dir.find_last_of(PHYSFS_getDirSeparator());
    dir = real_dir.substr(0,pos);

    m_log=new logger(this,0);
    m_log->log(LOG_LOG,"Initializing \"%s\"",title.c_str());

    m_log->log(LOG_LOG,"Base Directory: \"%s\"",PHYSFS_getBaseDir());
    m_log->log(LOG_LOG,"Directory: \"%s\"",dir.c_str());

    PHYSFS_mount(dir.c_str(), NULL, 0);

    output_versions();

    this->wnd = new window();

    if(!this->wnd->init(title, width, height))
    {
        delete wnd;
        return false;
    }

    this->gl_util = new opengl_util(m_log);

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
    m_log->log(LOG_LOG,"Exitting.");

    delete gl_util;
    delete wnd;
    delete m_log;

    if (!PHYSFS_deinit())
        std::cout<< "PHYSFS_deinit() failed!\n reason: " << PHYSFS_getLastError() << "." << std::endl;

    delete main_timer;
}

logger *application::get_logger()
{
    return m_log;
}

timer *application::get_timer()
{
    return main_timer;
}
