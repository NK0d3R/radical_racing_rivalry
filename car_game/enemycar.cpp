// Copyright 2018 Catalin G. Manciu

#include "enemycar.h"

void EnemyCar::reset(const FP32& z) {
    Car::reset(z);
    state = STATE_INVALID;
    setState(SWITCHING_GEARS);
}

void EnemyCar::setState(AIState newState) {
    if (state != newState) {
        state = newState;
        switch (state) {
            case SWITCHING_GEARS: {
                stateTimer = random(133, 333);
                setClutch(true);
                shiftGear(true);
            } break;
            case ACCELERATING: {
                gearShiftRPM = random(5000, 8000);
                setClutch(false);
            } break;
        }
    }
}

void EnemyCar::update(int16_t dt) {
    switch (state) {
        case SWITCHING_GEARS: {
            stateTimer -= dt;
            if (stateTimer <= 0) {
                setState(ACCELERATING);
            }
        } break;
        case ACCELERATING: {
            if (getRPM().getInt() > gearShiftRPM && getGear() < getMaxGear()) {
                setState(SWITCHING_GEARS);
            } else {
                pedalToTheMetal(true);
            }
        } break;
    }
    Car::update(dt);
}
