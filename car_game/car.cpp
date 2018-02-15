#include "defs.h"
#include "car.h"
#include "sprites.h"
#include "renderer.h"
#include "level.h"

#define BODY_ANIM       (0)
#define WHEELS_ANIM     (1)
#define REFLECTION_ANIM (2)

//Torque (in Nm)
const int32_t torque[] PROGMEM = {
    FP(225),                //1000
    FP(400),                //2000
    FP(450),                //3000
    FP(475),                //4000
    FP(495),                //5000
    FP(480),                //6000
    FP(450),                //7000
    FP(320)                 //8000
};

#define NB_TORQUES  (sizeof(torque)/sizeof(int16_t))

const int32_t gear_ratios[] PROGMEM = {
    FLOAT_TO_FP(9.13f),
    FLOAT_TO_FP(6.65f),
    FLOAT_TO_FP(4.45f),
    FLOAT_TO_FP(3.51f),
    FLOAT_TO_FP(2.89f)
};

#define WHEEL_CIRCUMFERENCE    FLOAT_TO_FP(2.0f * 3.141539f * 0.33f)
#define WHEEL_RADIUS           FLOAT_TO_FP(0.33f)
#define VEHICLE_MASS           FP(1800)

#define WIND_RESISTANCE        -FLOAT_TO_FP(0.02)
#define CONSTANT_RESISTANCE    -FP(150)

int32_t RPM2Torque(int32_t rpm) {
    if(rpm < 1000) {
        return pgm_read_dword(&torque[0]);
    }
    int32_t idx = (rpm / 1000) - 1;
    int32_t torque_start = pgm_read_dword(&torque[idx]);
    if(idx == (NB_TORQUES - 1)) {
        return torque_start;
    }
    ++idx;
    int32_t rpm_over = rpm - (idx * 1000);
    if(rpm_over == 0) {
        return torque_start;
    }
    int32_t torque_end = pgm_read_dword(&torque[idx]);
    return torque_start + 
                ((rpm_over * (torque_end - torque_start)) / 1000);
}

int32_t GetGearRatio(int16_t idx) {
    return pgm_read_dword(&gear_ratios[idx]);
}

void Car::Initialize(int16_t y) {
    y_pos = y;
    x_pos = 0;
    engine_rpm = 0;
    wheels_rpm = 0;
    speed = 0;
    gear = 1;
    throttle = 0;

    wheels.Init(GetSprite(SPRITE_CAR));
    reflection.Init(GetSprite(SPRITE_CAR));
    wheels.SetAnimation(WHEELS_ANIM, 0, true);
    reflection.SetAnimation(REFLECTION_ANIM, 0, true);
}

void Car::Draw(SpriteRenderer* renderer) {
    int32_t x = GetLevel().WorldToScreen(x_pos);
    GetSprite(SPRITE_CAR)->DrawAnimationFrame(renderer, BODY_ANIM,
                                              0, x, y_pos, 0);
    wheels.Draw(renderer, x - 12, y_pos);
    wheels.Draw(renderer, x + 13, y_pos);
    reflection.Draw(renderer, x, y_pos);
}

void Car::Accelerate(bool on) {
    if(on) {
        if(throttle < FP(1)) {
            throttle += FLOAT_TO_FP(0.1);

            if(throttle > FP(1)) {
                throttle = FP(1);
            }
        }
    } else {
        if(throttle > 0) {
            throttle -= FLOAT_TO_FP(0.1);

            if(throttle < 0) {
                throttle = 0;
            }
        }
    }
}

void Car::Update(int16_t dt) {
    int32_t wheel_speed = FP(speed) / (100 * WHEEL_CIRCUMFERENCE / 60);
    engine_rpm = FPTOI(GetGearRatio(0) * wheel_speed);
    if(FPTOI(engine_rpm) > 8000)
    {
        engine_rpm = FP(8000);
    }
    int32_t engine_torque = RPM2Torque(FPTOI(engine_rpm));
    int32_t forward_force = (throttle *
                             FPTOI(engine_torque * GetGearRatio(0))) /
                             WHEEL_RADIUS;
    forward_force += CONSTANT_RESISTANCE;
    int32_t accel = FP(forward_force) / VEHICLE_MASS;
    /*static int val = 1;
    if(val % 15 == 0)
    {
        Serial.println(wheel_speed);
        Serial.println(FPTOI(engine_rpm));
        Serial.println(engine_torque);
        Serial.println(forward_force);
        Serial.println(accel);
    }
    val ++;*/
    speed += (accel * dt) / 10;
    if(speed < 0) {
        speed = 0;
    }
    x_pos += FPTOI((speed * FP(dt)) / 1000);
    wheels.Update(dt);
    reflection.Update((dt >> 1));
}


