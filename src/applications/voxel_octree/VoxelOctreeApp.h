#ifndef VOXELOCTREEAPP_H
#define VOXELOCTREEAPP_H

#include "application/Application.h"
#include "gui/GuiEventListener.h"
#include "application/window.h"
#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "resources/ShaderLoader.h"
#include "scenegraph/Camera.h"
#include "voxel_octree/MortonOctree.h"
#include "voxel_octree/VoxMeshManager.h"

template <class T>
class TCubeMesh;

class VoxelOctreeApp: public Application
{
private:
    static VoxelOctreeApp * m_instance;
public:
    static VoxelOctreeApp * Instance();

    VoxelOctreeApp(uint32_t argc, const char ** argv);
    virtual ~VoxelOctreeApp();

    bool Init(const std::string & title, uint32_t width, uint32_t height);
    bool LoadLevel(const std::string & levelName);

    void InitResources();
    void InitPython();
    void AfterInit();
    virtual bool Update();
    void Exit();

    void OnWindowClose();

    ///Events
    void OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
    void OnMouseKey(int32_t button, int32_t action, int32_t mod);
    void OnMouseMove(double x, double y);
private:
    ShaderPtr sh;
    CameraPtr cam;
    MortonOctTree<10> * octree;
    VoxMeshManager * octreeGen;
    TCubeMesh<glm::vec3> * cube;
};


#endif // VOXELOCTREEAPP_H
