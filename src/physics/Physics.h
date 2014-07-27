#ifndef PHYSICS_H
#define PHYSICS_H

#define BIT(x) (1<<(x))

enum collisiontypes
{
    COL_NOTHING = 0,
    COL_NORMAL = BIT(1),
    COL_ARROW = BIT(2),
    COL_LEVEL_EDITOR = BIT(4),
    COL_SPECIAL1 = BIT(8),
    COL_SPECIAL2 = BIT(16)
};

#include "MotionState.h"
#include "CollisionHandler.h"
#include "PhysicsManager.h"
#endif
