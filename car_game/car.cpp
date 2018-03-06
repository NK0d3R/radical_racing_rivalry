// Copyright 2018 Catalin G. Manciu

#include "car.h"
#include "sprites.h"
#include "renderer.h"
#include "level.h"

#define BODY_ANIM       (0)
#define WHEELS_ANIM     (1)
#define REFLECTION_ANIM (2)

#define MIN_RPM         FP32(1100)
#define MAX_RPM         FP32(8000)

// Torque (in Nm)
constexpr FP32 g_torques[] PROGMEM = {
    FP32(225),                // 1000 RPM
    FP32(400),                // 2000 RPM
    FP32(450),                // 3000 RPM
    FP32(475),                // 4000 RPM
    FP32(495),                // 5000 RPM
    FP32(480),                // 6000 RPM
    FP32(450),                // 7000 RPM
    FP32(320)                 // 8000 RPM
};

#define NB_TORQUES  (sizeof(g_torques)/sizeof(FP32))

constexpr FP32 g_gearRatios[] PROGMEM = {
    FP32(0),
    FP32(9.13f),
    FP32(6.65f),
    FP32(4.45f),
    FP32(3.51f),
    FP32(2.89f)
};

#define WHEEL_CIRCUMFERENCE    FP32(2.0f * 3.141539f * 0.33f)
#define WHEEL_RADIUS           FP32(0.33f)
#define VEHICLE_MASS           FP32(1600)

#define WIND_RESISTANCE_MULT   -45
#define WIND_RESISTANCE_DIV    100
#define CONSTANT_RESISTANCE    FP32(-1500)
#define MAX_GEAR               ((sizeof(g_gearRatios)/sizeof(FP32)) - 1)

FP32 RPM2Torque(const FP32& rpm) {
    if (rpm < MIN_RPM) {
        return g_torques[0];
    } else if (rpm > MAX_RPM) {
        return FP32(0);
    }
    int32_t rpmI = rpm.getInt();
    int32_t idx = (rpmI / 1000);
    if (idx == (NB_TORQUES - 1)) {
        return FP32(&g_torques[idx]);
    }
    int32_t rpmOver = rpmI - (idx * 1000);
    if (rpmOver == 0) {
        return FP32(&g_torques[idx]);
    }
    FP32 tStart(&g_torques[idx]);
    FP32 tEnd(&g_torques[idx + 1]);
    return tStart + (((tEnd - tStart) * rpmOver) / 1000);
}

FP32 getGearRatio(int16_t idx) {
    return FP32(&g_gearRatios[idx]);
}

void Car::initialize(int16_t y) {
    yPos = y;
    xPos = 0;
    engineRPM = 0;
    wheelsRPM = 0;
    speed = 0;
    gear = 0;
    throttle = 0;
    clutch = false;

    wheels.init(GetSprite(SPRITE_CAR));
    reflection.init(GetSprite(SPRITE_CAR));
    wheels.setAnimation(WHEELS_ANIM, 0, true);
    reflection.setAnimation(REFLECTION_ANIM, 0, true);
}

void Car::shiftGear(bool up = true) {
    if (up == true) {
        if (gear < MAX_GEAR) {
            gear++;
        }
    } else {
        if (gear > 0) {
            gear--;
        }
    }
}

void Car::draw(SpriteRenderer* renderer) {
    int32_t x = GetLevel().worldToScreen(xPos);
    GetSprite(SPRITE_CAR)->drawAnimationFrame(renderer, BODY_ANIM,
                                              0, x, yPos, 0);
    wheels.draw(renderer, x - 12, yPos);
    wheels.draw(renderer, x + 13, yPos);
    reflection.draw(renderer, x, yPos);
}

void Car::pedalToTheMetal(bool on) {
    if (on) {
        FP32 maxThrottle(1);
        if (throttle < maxThrottle) {
            throttle += FP32(0.1);
            throttle.clampUpper(maxThrottle);
        }
    } else {
        FP32 minThrottle(0);
        if (throttle > minThrottle) {
            throttle -= FP32(0.1);
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

    if (engineConnected) {
        wheelsRPM = (speed * 60) / WHEEL_CIRCUMFERENCE;
        targetEngineRPM = getGearRatio(gear) * wheelsRPM;
        engineRPM = (targetEngineRPM + engineRPM) / 2;
        engineTorque = RPM2Torque(engineRPM.getInt());
        forwardForce = (throttle * engineTorque * getGearRatio(gear)) /
                        WHEEL_RADIUS;
    } else {
        targetEngineRPM = (MIN_RPM) + (throttle * MAX_RPM);
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
    CLAMP_LOWER(speed, FP32(0));
    xPos += (speed * fpDT) / 1000;
}

void Car::updateWheelsAnim(int16_t dt) {
    int16_t newDT = ((wheelsRPM * (int32_t)dt) / 125).getInt();
    CLAMP_UPPER(newDT, 200);
    wheels.update(newDT);
}

void Car::update(int16_t dt) {
    updateEngine(dt);
    updateWheelsAnim(dt);
    reflection.update(dt >> 1);
}


