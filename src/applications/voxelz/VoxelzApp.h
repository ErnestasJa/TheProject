#ifndef VOXELZ_H
#define VOXELZ_H

#include "application/Application.h"
#include "gui/GUIEventListener.h"

class VoxelzApp: public Application
{
public:
    VoxelzApp(uint32_t argc, const char ** argv);
    virtual ~VoxelzApp();

    bool Init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool Update();
    void HandleMovement(float delta);
    void Exit();

    void OnWindowClose();

    ///Events
    void OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
    void OnMouseKey(int32_t button, int32_t action, int32_t mod);
    void OnMouseMove(double x, double y);

    std::string GetApplicationId()
    {
        return "Voxelz";
    }

protected:
};

#endif // VOXELZ_H
