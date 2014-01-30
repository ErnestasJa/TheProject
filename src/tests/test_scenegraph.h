#ifndef TEST_SCENEGRAPH_H
#define TEST_SCENEGRAPH_H

#include "application/application.h"

namespace sg{ class scenegraph; struct sg_material;}

struct shader; class shader_loader;
class texture; class graphics_manager;
class mesh_loader; struct mesh;

class test_scenegraph: public application
{
protected:
    graphics_manager*   m_graphics_manager;
    mesh_loader     *   m_mesh_loader;
    shader_loader   *   m_shader_loader;
    sg::scenegraph  *   m_scenegraph;

public:
    test_scenegraph(uint32_t argc, const char ** argv);
    ~test_scenegraph();

    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();

    bool init_scene();

protected:
    void cam_move();
};


#endif // TEST_SCENEGRAPH_H
