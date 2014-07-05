#ifndef SG_SCENEGRAPH_LOADER_H
#define SG_SCENEGRAPH_LOADER_H
struct AppContext;
namespace sg
{

class sg_sg_scenegraph_loader
{
public:
    sg_sg_scenegraph_loader();
    virtual ~sg_sg_scenegraph_loader();

    bool load_scene(AppContext * app_ctx, const std::string & filename,bool with_physics);

protected:

};

}

#endif // SG_SCENEGRAPH_LOADER_H
