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

    virtual bool Init();
    virtual bool Update();
    virtual bool Exit();

    void InitOctree();
    void BuildOctree();
    void FreeOctree();

    ///Events
    void OnWindowClose();
    void OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
    void OnMouseKey(int32_t button, int32_t action, int32_t mod);
    void OnMouseMove(double x, double y);

    virtual std::string GetApplicationId();
    virtual void InitVariables();
    
private:
    MeshPtr mesh;
    CameraPtr cam;
    MortonOctTreePtr octree;
    VoxMeshManager * octreeGen;
    TimerPtr timer;
};

#endif // VOXELOCTREEAPP_H
