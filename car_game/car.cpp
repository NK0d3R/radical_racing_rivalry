#include "defs.h"
#include "car.h"
#include "sprites.h"
#include "renderer.h"
#include "level.h"

#define BODY_ANIM       (0)
#define WHEELS_ANIM     (1)
#define REFLECTION_ANIM (2)

#define MIN_RPM         (600)
#define MAX_RPM         (8000)
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
    0,
    FLOAT_TO_FP(9.13f),
    FLOAT_TO_FP(6.65f),
    FLOAT_TO_FP(4.45f),
    FLOAT_TO_FP(3.51f),
    FLOAT_TO_FP(2.89f)
};

#define WHEEL_CIRCUMFERENCE    FLOAT_TO_FP(2.0f * 3.141539f * 0.33f)
#define WHEEL_RADIUS           FLOAT_TO_FP(0.33f)
#define VEHICLE_MASS           FP(1600)

#define WIND_RESISTANCE_MULT   -45
#define WIND_RESISTANCE_DIV    100
#define CONSTANT_RESISTANCE    -FP(1500)
#define MAX_GEAR               ((sizeof(gear_ratios)/sizeof(int32_t)) - 1)

int32_t RPM2Torque(int32_t rpm) {
    if(rpm < MIN_RPM) {
        return (pgm_read_dword(&torque[0]));
    } else if(rpm > MAX_RPM) {
        return 0;
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
    gear = 0;
    throttle = 0;
    clutch = false;

    wheels.Init(GetSprite(SPRITE_CAR));
    reflection.Init(GetSprite(SPRITE_CAR));
    wheels.SetAnimation(WHEELS_ANIM, 0, true);
    reflection.SetAnimation(REFLECTION_ANIM, 0, true);
}

void Car::ShiftGear(bool up = true) {
    if(up == true) {
        if(gear < MAX_GEAR) {
            gear ++;
        }
    } else {
        if(gear > 0) {
            gear --;
        }
    }
}

void Car::Clutch(bool on) {
    clutch = on;
}

void Car::Draw(SpriteRenderer* renderer) {
    int32_t x = GetLevel().WorldToScreen(x_pos);
    GetSprite(SPRITE_CAR)->DrawAnimationFrame(renderer, BODY_ANIM,
                                              0, x, y_pos, 0);
    wheels.Draw(renderer, x - 12, y_pos);
    wheels.Draw(renderer, x + 13, y_pos);
    reflection.Draw(renderer, x, y_pos);
}

void Car::PedalToTheMetal(bool on) {
    if(on) {
        if(throttle < FP(1)) {
            throttle += FLOAT_TO_FP(0.1);
            CLAMP_UPPER(throttle, FP(1));
        }
    } else {
        if(throttle > FLOAT_TO_FP(0)) {
            throttle -= FLOAT_TO_FP(0.1);
            CLAMP_LOWER(throttle, 0);
        }
    }
}

void Car::UpdateEngine(int16_t dt) {
    bool engine_connected = EngineIsConnected();

    int32_t forward_force = 0;
    int32_t target_engine_rpm = 0;
    int32_t engine_torque = 0;

    if(engine_connected) {
        wheels_rpm = (FP(speed) * 60) / WHEEL_CIRCUMFERENCE;
        target_engine_rpm = FPTOI(GetGearRatio(gear) * wheels_rpm);
        engine_rpm = ((target_engine_rpm + engine_rpm) >> 1);
        engine_torque = RPM2Torque(FPTOI(engine_rpm));
        forward_force = (throttle *
                          FPTOI(engine_torque * GetGearRatio(gear))) /
                          WHEEL_RADIUS;
    } else {
        target_engine_rpm = FP(MIN_RPM) + (throttle * MAX_RPM);
        if(engine_rpm < target_engine_rpm) {
           engine_rpm =  ((target_engine_rpm + engine_rpm) >> 1);
        } else {
           engine_rpm =  ((target_engine_rpm + engine_rpm) >> 1);
        }
    }
    forward_force += CONSTANT_RESISTANCE +
                     FPTOI((WIND_RESISTANCE_MULT * speed * speed) /
                     WIND_RESISTANCE_DIV);
    int32_t accel = FP(forward_force) / VEHICLE_MASS;
    speed += FPTOI((accel * FP(dt)) / 1000);
    CLAMP_LOWER(speed, 0);
    x_pos += FPTOI((speed * FP(dt)) / 1000);
}

void Car::UpdateWheelsAnim(int16_t dt) {
    int16_t newdt = FPTOI((dt * wheels_rpm) / 125);
    CLAMP_UPPER(newdt, 200);
    wheels.Update(newdt);
}

void Car::Update(int16_t dt) {
    UpdateEngine(dt);
    UpdateWheelsAnim(dt);
    reflection.Update((dt >> 1));
}


