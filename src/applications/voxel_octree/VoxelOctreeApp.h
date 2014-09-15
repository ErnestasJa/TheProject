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
#include "voxel_octree/MortonOctTree.h"
#include "voxel_octree/VoxMeshManager.h"

class VoxelOctreeApp: public Application
{
public:
    VoxelOctreeApp(uint32_t argc, const char ** argv);
    virtual ~VoxelOctreeApp();

    bool Init(const std::string & title, uint32_t width, uint32_t height);
    void InitPlaneMesh();
    virtual bool Update();
    void Exit();

    void OnWindowClose();

    ///Events
    void OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
    void OnMouseKey(int32_t button, int32_t action, int32_t mod);
    void OnMouseMove(double x, double y);
private:
    MeshPtr mesh;
    ShaderPtr sh;
    CameraPtr cam;
    MortonOctTree<10> * octree;
    VoxMeshManager * octreeGen;
};

#endif // VOXELOCTREEAPP_H
