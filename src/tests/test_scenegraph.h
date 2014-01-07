#ifndef TEST_SCENEGRAPH_H
#define TEST_SCENEGRAPH_H

#include "application/application.h"

namespace sg{ class scenegraph;}


class texture;
struct shader;
class image_loader;
class iqmloader;
struct mesh;

class test_scenegraph: public application
{
protected:
    std::vector<std::shared_ptr<mesh> > * mesh_cache;
    std::vector<std::shared_ptr<shader> > * shader_cache;
    std::vector<std::shared_ptr<texture> > * tex_cache;

    ///matrices
    glm::mat4 M, V, P, MVP;

    image_loader *  m_image_loader;
    iqmloader *     m_iqm_loader;

    sg::scenegraph * m_scenegraph;

    std::weak_ptr<shader> m_shader;

public:
    test_scenegraph(uint32_t argc, const char ** argv);
    ~test_scenegraph();

    bool init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool update();
    void exit();

protected:
    void init_gl();
    void cam_move();
};


#endif // TEST_SCENEGRAPH_H
