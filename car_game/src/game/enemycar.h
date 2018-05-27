// Copyright 2018 Catalin G. Manciu

#ifndef ENEMYCAR_H_
#define ENEMYCAR_H_

#include "car.h"

class EnemyCar final : public Car {
 public:
    EnemyCar(Level* p, FP32 initialX, FP32 initialY, uint8_t scrW) :
        Car(p, initialX, initialY, scrW) {}
    virtual void reset(const FP32& z);
    virtual void update(int16_t dt);
    virtual void onRaceStart();
 private:
    int16_t stateTimer;
    int16_t gearShiftRPM;

    enum AIState : uint8_t {
        Invalid,
        Wait,
        SwitchingGears,
        Accelerating,
        Dead
    };

    AIState state;
    void setState(AIState newState);
};

#endif  // ENEMYCAR_H_

