#pragma once
#include "Unit.h"
#include "Behavior.h"

float GetElapsedTime();

class UnitMoveBehavior : public IBehavior
{
protected:
    UnitMovement* m_Movement;

    void Init(Unit* u, Vector3 target) {
        m_Movement = new UnitMovement(u);
        m_Movement->m_Target = target;
    }

    virtual BehaviorStatus Update() override
    {
        m_Movement->Update(GetElapsedTime());

        if (m_Movement->isMoving()) 
            return BH_RUNNING;
        else 
            return BH_SUCCESS;
    }
};


void 


