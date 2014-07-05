#ifndef MATERIALTEST_H
#define MATERIALTEST_H

#include "application/Application.h"
#include "gui/GuiEventListener.h"

#include "scenegraph/SGScenegraph.h"
#include "scenegraph/SGGraphicsManager.h"

class MaterialTest: public Application
{
public:
    MaterialTest(uint32_t argc, const char ** argv);
    virtual ~MaterialTest();

    bool Init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool Update();
    void Exit();

    void OnWindowClose();

protected:
};

#endif // MATERIALTEST_H
