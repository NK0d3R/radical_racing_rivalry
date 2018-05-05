// Copyright 2018 Catalin G. Manciu

#ifndef GEARSHIFT_H_
#define GEARSHIFT_H_

#include "../stdinc.h"

class SpriteRenderer;

class GearShift {
 public:
    virtual void reset() = 0;

    virtual int8_t getShiftResult() = 0;

    virtual void onUp() = 0;
    virtual void onDown() = 0;
    virtual void onLeft() = 0;
    virtual void onRight() = 0;

    virtual void update() = 0;
    virtual void draw(SpriteRenderer* renderer, int16_t x, int16_t y) = 0;
};

class GearShiftAuto final : public GearShift {
 private:
    int8_t currentGear;
    int8_t scrollAmount;
    uint8_t frameCounter;

    enum ShiftState : int8_t {
        Idle = 0,
        ShiftUp = 1,
        ShiftDown = -1
    };

    ShiftState state;
 public:
    virtual int8_t getShiftResult() { return state == Idle ? currentGear : -1; }

    virtual void onUp();
    virtual void onDown();
    virtual void onLeft()  {}
    virtual void onRight() {}

    virtual void update();
    virtual void draw(SpriteRenderer* renderer, int16_t x, int16_t y);
    virtual void reset();
};

class GearShiftManual final : public GearShift {
 private:
    int8_t currentGear;
    int8_t currentX;
    int8_t currentY;
    int8_t currentOffset;
    int8_t targetOffset;
    enum ShiftState : int8_t {
        Idle = 0,
        Left,
        Right,
        Up,
        Down
    };

    ShiftState state;
    int8_t getGearAtPosition(int8_t x, int8_t y);
    bool canGoDirection(ShiftState direction);
    void tryGoDirection(ShiftState direction);
    void getNextPosition(ShiftState direction, int8_t* x, int8_t* y);

 public:
    virtual int8_t getShiftResult() {
        return state == Idle ? getGearAtPosition(currentX, currentY) : -1; }

    virtual void onUp()     { tryGoDirection(Up); }
    virtual void onDown()   { tryGoDirection(Down); }
    virtual void onLeft()   { tryGoDirection(Left); }
    virtual void onRight()  { tryGoDirection(Right); }

    virtual void update();
    virtual void draw(SpriteRenderer* renderer, int16_t x, int16_t y);
    virtual void reset();
};

#endif  // GEARSHIFT_H_
