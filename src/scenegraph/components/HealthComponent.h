#ifndef HEALTHCOMPONENT_H
#define HEALTHCOMPONENT_H
#include "Component.h"

namespace sg
{
class HealthComponent: public Component
{
    public:
        HealthComponent(isg_object * obj);
        virtual ~HealthComponent();

        virtual void OnUpdate(float deltaTime);
        virtual uint32_t GetType();

        void SetMaxHealth(int maxHealth);
        void SetHealth(int health);
        int GetHealth();
        bool IsAlive();

    protected:
        int _health,
            _maxHealth;
};
}
#endif // HEALTHCOMPONENT_H
