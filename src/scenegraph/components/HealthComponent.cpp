#include "precomp.h"
#include "HealthComponent.h"

namespace sg
{
HealthComponent::HealthComponent(isg_object * obj): Component(obj)
{
    //ctor
}

HealthComponent::~HealthComponent()
{
    //dtor
}

void HealthComponent::OnUpdate(float deltaTime)
{

}

uint32_t HealthComponent::GetType()
{
    return HEALTH_COMPONENT;
}

void HealthComponent::SetMaxHealth(int maxHealth)
{
    _maxHealth = maxHealth;
}

void HealthComponent::SetHealth(int health)
{
    _health = clamp(health,0,_maxHealth);
}

int HealthComponent::GetHealth()
{
    return _health;
}

bool HealthComponent::IsAlive()
{
    return _health > 0;
}
}
