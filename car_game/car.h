// Copyright 2018 Catalin G. Manciu

#ifndef CAR_H_
#define CAR_H_

#include "stdinc.h"
#include "defs.h"
#include "ardusprite.h"
#include "sprites.h"

class SpriteRenderer;
class Level;
class Car {
 public:
    Car() {}
    void initialize(int16_t y);
    void draw(SpriteRenderer* renderer);
    void update(int16_t dt);
    FP32& getX()                        { return xPos; }
    FP32& getRPM()                      { return engineRPM; }
    FP32& getSpeed()                    { return speed; }
    int8_t getGear()                    { return gear; }
    void shiftGear(bool up = true);
    void pedalToTheMetal(bool on);
    void setClutch(bool on)             { clutch = on; }
    bool isClutched()                   { return clutch; }

 private:
    FP32 xPos;      // In world coords
    int16_t yPos;   // In screen coords
    int8_t  gear;
    FP32 engineRPM;
    FP32 wheelsRPM;
    FP32 speed;
    FP32 throttle;
    bool clutch;
    SpriteAnimator reflection;
    SpriteAnimator wheels;
    void updateEngine(int16_t dt);
    void updateWheelsAnim(int16_t dt);
    inline bool engineIsConnected() { return gear > 0 &&
                                                clutch == false; }
};

#endif  // CAR_H_

