#ifndef COLLISIONTESTAPP_H
#define COLLISIONTESTAPP_H

#include <application/Application.h>


class CollisionTestApp : public Application
{
public:
    CollisionTestApp(uint32_t argc, const char ** argv);
    virtual ~CollisionTestApp();
    bool Init(const std::string & title, uint32_t width, uint32_t height);
    virtual bool Update();
    void Exit();

    void OnWindowClose();

    ///Events
    void OnKeyEvent(int32_t key, int32_t scan_code, int32_t action, int32_t modifiers);
    void OnMouseKey(int32_t button, int32_t action, int32_t mod);
    void OnMouseMove(double x, double y);

protected:
};

#endif // COLLISIONTESTAPP_H
