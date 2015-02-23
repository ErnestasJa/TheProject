#ifndef VOXMESHEDITORAPP_H
#define VOXMESHEDITORAPP_H

#include <application/Application.h>
#include <opengl/Shader.h>
#include <opengl/Mesh.h>
#include <GUI/GUIEventListener.h>

class VoxMeshGrid;
class GridMesh;

class ParticleSystem;

class GUIEnvironment;
class GUIWindow;
class GUIColorPicker;

class GUIStaticText;

class GUIButton;
class GUISlider;
class GUICheckbox;

class VoxelMesh;

class Camera;

class ParticleEmitter;

class VoxMeshEditorApp : public Application, public GUIEventListener
{
private:
    ShaderPtr _defaultShader,_meshShader,_voxShader,_particleShader;
    ParticleSystem *_particleSystem;
    VoxMeshGrid * _grid;
    GridMesh * _axisGrid;

    MeshPtr _iqmMesh;
    VoxelMesh* _voxMesh;

    GUIEnvironment* _guiEnv;
    GUIWindow* _mainWin;
    GUIColorPicker* _colPicker;

    Camera* _cam;

    ParticleEmitter* _emitter;
    std::map<std::string,GUIButton*> _buttons;
    std::map<std::string,GUIStaticText*> _labels;
    std::map<std::string,GUISlider*> _sliders;
    std::map<std::string,GUICheckbox*> _checkboxes;
    std::map<std::string,bool> _guiSwitches;
public:
    VoxMeshEditorApp(uint32_t argc, const char ** argv);
    virtual ~VoxMeshEditorApp();
    bool Init(const std::string & title, uint32_t width, uint32_t height);
    void InitGUI();
    virtual bool Update();
    void HandleMovement(float dt);
    void Exit();

    void OnWindowClose();

    ///Events
    void OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
    void OnMouseKey(int32_t button, int32_t action, int32_t mod);
    void OnMouseMove(double x, double y);

    bool OnEvent(const GUIEvent & e);

protected:
};

#endif // VOXMESHEDITORAPP_H
