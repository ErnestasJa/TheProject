#ifndef VOXELOCTREEBENCHMARKAPP_H
#define VOXELOCTREEBENCHMARKAPP_H

#include "application/Application.h"
#include "application/Window.h"
#include "opengl/Mesh.h"
#include "opengl/Shader.h"
#include "opengl/MVar.h"
#include "resources/ShaderLoader.h"
#include "scenegraph/Camera.h"
#include "motree/MortonOctree.h"
#include "motree/VoxMeshManager.h"

class VoxelOctreeBenchmarkApp: public Application
{
public:
    VoxelOctreeBenchmarkApp(uint32_t argc, const char ** argv);
    virtual ~VoxelOctreeBenchmarkApp();

    bool Init(const std::string & title, uint32_t width, uint32_t height);
    void InitOctree();
    void BuildOctree();
    void FreeOctree();
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
    MortonOctTreePtr octree;
    VoxMeshManager * octreeGen;
    timer_ptr timer;
};

#endif // VOXELOCTREEAPP_H
