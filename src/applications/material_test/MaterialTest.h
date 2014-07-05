#ifndef MATERIALTEST_H
#define MATERIALTEST_H

#include "application/application.h"
#include "gui/gui_event_listener.h"

#include "scenegraph/sg_scenegraph.h"
#include "scenegraph/sg_graphics_manager.h"

class MaterialTest: public application
{
public:
    MaterialTest(uint32_t argc, const char ** argv);
    virtual ~MaterialTest();

    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();

    void on_window_close();

protected:
};

#endif // MATERIALTEST_H
