#ifndef SG_SCENEGRAPH_LOADER_H
#define SG_SCENEGRAPH_LOADER_H
struct app_context;
namespace sg
{

class sg_scenegraph_loader
{
public:
    sg_scenegraph_loader();
    virtual ~sg_scenegraph_loader();

    bool load_scene(app_context * app_ctx, const std::string & filename,bool with_physics);

protected:

};

}

#endif // SG_SCENEGRAPH_LOADER_H
