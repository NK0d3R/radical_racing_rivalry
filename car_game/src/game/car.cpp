// Copyright 2018 Catalin G. Manciu

#include "../defs.h"
#include "car.h"
#include "level.h"
#include "../res/sprites.h"
#include "../engine/renderer.h"

// Torque (in Nm)
constexpr FP32 kTorques[] PROGMEM = {
    FP32(225),                // 1000 RPM
    FP32(400),                // 2000 RPM
    FP32(450),                // 3000 RPM
    FP32(475),                // 4000 RPM
    FP32(495),                // 5000 RPM
    FP32(480),                // 6000 RPM
    FP32(450),                // 7000 RPM
    FP32(320)                 // 8000 RPM
};

#define NB_TORQUES  (sizeof(kTorques)/sizeof(FP32))

constexpr FP32 kGearRatios[] PROGMEM = {
    FP32(0),
    FP32(9.13f),
    FP32(6.65f),
    FP32(4.45f),
    FP32(3.90f),
    FP32(3.30f)
};

#define WHEEL_CIRCUMFERENCE    FP32(2.0f * 3.141539f * 0.33f)
#define WHEEL_RADIUS           FP32(0.33f)
#define VEHICLE_MASS           FP32(1600)

#define WIND_RESISTANCE_MULT   -32
#define WIND_RESISTANCE_DIV    100
#define CONSTANT_RESISTANCE    FP32(-1200)

FP32 RPM2Torque(const FP32& rpm) {
    if (rpm < Defs::MinRPM) {
        return FP32(static_cast<const void*>(&kTorques[0]));
    } else if (rpm > Defs::MaxRPM) {
        return FP32(0);
    }
    int32_t rpmI = rpm.getInt();
    int32_t idx = (rpmI / 1000);
    if (idx == (NB_TORQUES - 1)) {
        return FP32(static_cast<const void*>(&kTorques[idx]));
    }
    int32_t rpmOver = rpmI - (idx * 1000);
    if (rpmOver == 0) {
        return FP32(static_cast<const void*>(&kTorques[idx]));
    }
    FP32 tStart(static_cast<const void*>(&kTorques[idx]));
    FP32 tEnd(static_cast<const void*>(&kTorques[idx + 1]));
    return tStart + (((tEnd - tStart) * rpmOver) / 1000);
}

FP32 getGearRatio(int16_t idx) {
    return FP32(&kGearRatios[idx]);
}

void Car::reset(const FP32& z) {
    xPos = 0;
    zPos = z;
    engineRPM = 0;
    wheelsRPM = 0;
    speed = 0;
    gear = 0;
    throttle = 0;
    overheatCounter = 0;
    clutch = false;
    alive = true;
    lastReflectionPos = 0;

    wheels.init(GetSprite(Defs::SpriteCar));
    reflection.init(GetSprite(Defs::SpriteCar));
    explosion.init(GetSprite(Defs::SpriteCar));
    wheels.setAnimation(Defs::AnimCarWheels, 0, true);
}

void Car::shiftGear(bool up = true) {
    if (up == true) {
        if (gear < Defs::MaxGear) {
            gear++;
        }
    } else {
        if (gear > 0) {
            gear--;
        }
    }
}

void Car::destroy() {
     alive = false;
     explosion.setAnimation(Defs::AnimCarExplosion, 0, false);
}

void Car::draw(SpriteRenderer* renderer) {
    GetSprite(Defs::SpriteCar)->drawAnimationFrame(renderer, Defs::AnimCarBody,
                                              0, screenX, screenY, 0);
    wheels.draw(renderer, screenX - 34, screenY);
    wheels.draw(renderer, screenX - 10, screenY);
    if (reflection.animPlaying()) {
        reflection.draw(renderer, screenX, screenY);
    }
    if (explosion.animPlaying()) {
        explosion.draw(renderer, screenX, screenY);
    }
}

void Car::pedalToTheMetal(bool on) {
    if (on) {
        FP32 maxThrottle(1);
        if (throttle < maxThrottle) {
            throttle += FP32(0.1f);
            throttle.clampUpper(maxThrottle);
        }
    } else {
        FP32 minThrottle(0);
        if (throttle > minThrottle) {
            throttle -= FP32(0.1f);
            throttle.clampLower(minThrottle);
        }
    }
}

void Car::updateEngine(int16_t dt) {
    bool engineConnected = engineIsConnected();

    FP32 forwardForce = 0;
    FP32 targetEngineRPM = 0;
    FP32 engineTorque = 0;
    FP32 fpDT = FP32(dt);

    wheelsRPM = (speed * 60) / WHEEL_CIRCUMFERENCE;

    if (engineConnected) {
        targetEngineRPM = getGearRatio(gear) * wheelsRPM;
        engineRPM = (targetEngineRPM + engineRPM) / 2;
        engineTorque = RPM2Torque(engineRPM.getInt());
        forwardForce = (throttle * engineTorque * getGearRatio(gear)) /
                        WHEEL_RADIUS;
    } else {
        targetEngineRPM = (Defs::MinRPM) + (throttle * Defs::MaxRPM);
        if (engineRPM < targetEngineRPM) {
            engineRPM = (targetEngineRPM + engineRPM) / 2;
        } else {
            engineRPM = (targetEngineRPM + engineRPM * 4) / 5;
        }
    }

    forwardForce += CONSTANT_RESISTANCE +
                     (speed * speed * WIND_RESISTANCE_MULT) /
                     WIND_RESISTANCE_DIV;
    FP32 accel = forwardForce / VEHICLE_MASS;
    speed += (accel * fpDT) / 1000;
    speed.clampLower(FP32(0));
    xPos += (speed * fpDT) / 1000;

    if (alive) {
        int32_t engineRPMi = engineRPM.getInt();
        if (engineRPMi > 7200) {
            overheatCounter += (engineRPMi - 7000) / 200;
            if (overheatCounter > 30) {
                destroy();
            }
        } else {
            overheatCounter = 0;
        }
    }
}

void Car::updateWheelsAnim(int16_t dt) {
    int16_t newDT = ((wheelsRPM * (int32_t)dt) / 125).getInt();
    newDT = Utils::upperClamp(newDT, 200);
    wheels.update(newDT);
}

void Car::updateReflectionAnim(int16_t dt) {
    int8_t crtReflectionPos = xPos.getInt() / 250;
    if (lastReflectionPos != crtReflectionPos) {
        reflection.setAnimation(Defs::AnimCarReflection, 0, false);
        lastReflectionPos = crtReflectionPos;
    }
    reflection.update(((Utils::mpsToKph(speed) * (int32_t)dt) / 120).getInt());
}

void Car::update(int16_t dt) {
    updateEngine(dt);
    updateWheelsAnim(dt);
    updateReflectionAnim(dt);
    explosion.update(dt);
}
