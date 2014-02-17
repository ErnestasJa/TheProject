#pragma once

#include "application/application.h"

class gui_environment;
class tinyxml_test_application: public application
{
protected:
    gui_environment* env;
public:
    tinyxml_test_application(uint32_t argc, const char ** argv);
    ~tinyxml_test_application();
    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();
};
