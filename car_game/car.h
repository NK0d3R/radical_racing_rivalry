#ifndef __CAR_H__
#define __CAR_H__

#include "defs.h"
#include "ardusprite.h"
#include "sprites.h"

class SpriteRenderer;
class Level;
class Car {
    public:
        void Initialize(int16_t y);
        void Draw(SpriteRenderer* renderer);
        void Update(int16_t dt);
        int32_t GetX()      { return x_pos; }
        int32_t GetRPM()    { return engine_rpm; }
        int32_t GetSpeed()  { return speed; }
        void Accelerate(bool on);
    private:
        int32_t x_pos;  //In world coords
        int16_t y_pos;  //In screen coords
        int8_t  gear;
        int32_t engine_rpm;
        int32_t wheels_rpm;
        int32_t speed;
        int32_t throttle;
        SpriteAnimator reflection;
        SpriteAnimator wheels;
};

#endif

