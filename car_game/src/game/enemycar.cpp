// Copyright 2018 Catalin G. Manciu

#include "enemycar.h"

void EnemyCar::reset(const FP32& z) {
    Car::reset(z);
    state = Invalid;
    setState(Wait);
}

void EnemyCar::setState(AIState newState) {
    if (state != newState) {
        switch (newState) {
            case SwitchingGears: {
                stateTimer = random(133, 333);
                setClutch(true);
                shiftGear(true);
            } break;
            case Accelerating: {
                gearShiftRPM = 7200 + random(-600, 300);
                setClutch(false);
            } break;
        }
        state = newState;
    }
}

void EnemyCar::update(int16_t dt) {
    switch (state) {
        case SwitchingGears: {
            stateTimer -= dt;
            if (stateTimer <= 0) {
                setState(Accelerating);
            }
        } break;
        case Accelerating: {
            if (getRPM().getInt() > gearShiftRPM && getGear() < getMaxGear()) {
                setState(SwitchingGears);
            }
        } break;
    }
    pedalToTheMetal(state == Accelerating);
    if (!alive) {
        setState(Dead);
    }
    Car::update(dt);
}

void EnemyCar::onRaceStart() {
    setState(SwitchingGears);
}
