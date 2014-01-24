#ifndef TEST_SCENEGRAPH_H
#define TEST_SCENEGRAPH_H

#include "application/application.h"

namespace sg{ class scenegraph; struct sg_material;}

struct shader; class shader_loader;
class texture; class image_loader;
class iqmloader; struct mesh;

class test_scenegraph: public application
{
protected:
    std::vector<std::shared_ptr<mesh> > * mesh_cache;
    std::vector<std::shared_ptr<texture> > * tex_cache;

    ///matrices
    image_loader *  m_image_loader;
    iqmloader *     m_iqm_loader;
    shader_loader * m_shader_loader;

    sg::scenegraph * m_scenegraph;

public:
    test_scenegraph(uint32_t argc, const char ** argv);
    ~test_scenegraph();

    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();

protected:
    void cam_move();
};


#endif // TEST_SCENEGRAPH_H
