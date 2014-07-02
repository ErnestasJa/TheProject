#pragma once

#include "Application/Application.h"

class gui_environment;
class tinyxml_test_Application: public Application
{
protected:
    gui_environment* env;
public:
    tinyxml_test_Application(uint32_t argc, const char ** argv);
    ~tinyxml_test_Application();
    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();
};
