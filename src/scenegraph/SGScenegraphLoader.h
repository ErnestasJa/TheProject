#ifndef SG_SCENEGRAPH_LOADER_H
#define SG_SCENEGRAPH_LOADER_H
struct AppContext;
namespace sg
{

class ScenegraphLoader
{
public:
    ScenegraphLoader();
    virtual ~ScenegraphLoader();

    bool LoadScene(AppContext * app_ctx, const std::string & filename,bool with_physics);

protected:

};

}

#endif // SG_SCENEGRAPH_LOADER_H
