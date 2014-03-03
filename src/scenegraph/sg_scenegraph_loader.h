#ifndef SG_SCENEGRAPH_LOADER_H
#define SG_SCENEGRAPH_LOADER_H

namespace sg
{
class scenegraph;
class sg_scenegraph_loader
{
public:
    sg_scenegraph_loader();
    virtual ~sg_scenegraph_loader();

    bool load_scene(scenegraph * sg, const std::string & filename);

protected:

};

}

#endif // SG_SCENEGRAPH_LOADER_H
