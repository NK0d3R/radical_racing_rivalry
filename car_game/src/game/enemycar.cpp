// Copyright 2018 Catalin G. Manciu

#include "enemycar.h"

void EnemyCar::reset(const FP32& z) {
    Car::reset(z);
    state = Invalid;
    setState(Wait);
}

void EnemyCar::setState(AIState newState) {
    if (state != newState) {
        state = newState;
        switch (state) {
            case SwitchingGears: {
                stateTimer = random(133, 333);
                setClutch(true);
                shiftGear(true);
            } break;
            case Accelerating: {
                gearShiftRPM = random(5000, 8000);
                setClutch(false);
            } break;
        }
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
            } else {
                pedalToTheMetal(true);
            }
        } break;
    }
    Car::update(dt);
}

void EnemyCar::onRaceStart() {
    setState(SwitchingGears);
}
