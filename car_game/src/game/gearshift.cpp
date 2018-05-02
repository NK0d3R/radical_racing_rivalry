// Copyright 2018 Catalin G. Manciu

#include "../defs.h"
#include "gearshift.h"
#include "../res/sprites.h"
#include "../engine/renderer.h"

#define GEAR_DISPL_W            (8)
#define GEAR_DISPL_H            (7)
#define GEAR_SCROLL_FRAMES      (3)
#define GEAR_SCROLL_PER_FRAME   (GEAR_DISPL_H / GEAR_SCROLL_FRAMES)

void GearShiftAuto::reset() {
    scrollAmount = 0;
    frameCounter = 0;
    currentGear = 0;
    state = Idle;
}

void GearShiftAuto::update() {
    frameCounter++;
    if (state != Idle) {
        scrollAmount += GEAR_SCROLL_PER_FRAME;
        if (scrollAmount >= GEAR_DISPL_H) {
            currentGear += (static_cast<int16_t>(state));
            scrollAmount = 0;
            state = Idle;
        }
    }
}

void GearShiftAuto::draw(SpriteRenderer* renderer, int16_t x, int16_t y) {
    int8_t arrowOffset = (frameCounter >> 1) & 0x3;
    Sprite* car = GetSprite(SPRITE_CAR);
    car->drawAnimationFrame(renderer, CAR_GEARS_AUTO, HUD_GEARS_M_BG, x, y, 0);
    if (currentGear < MAX_GEAR) {
        car->drawAnimationFrame(renderer, CAR_GEARS_AUTO, HUD_GEARS_M_UP, x,
                                y - arrowOffset, 0);
    }
    if (currentGear > 0) {
        car->drawAnimationFrame(renderer, CAR_GEARS_AUTO, HUD_GEARS_M_DOWN, x,
                                y + arrowOffset, 0);
    }
    renderer->setClip(x - (GEAR_DISPL_W >> 1), y - (GEAR_DISPL_H >> 1),
                      GEAR_DISPL_W, GEAR_DISPL_H);
    if (state != Idle) {
        y = y + (static_cast<int16_t>(state)) * scrollAmount;
    }
    car->drawAnimationFrame(renderer, CAR_GEARS_AUTO, currentGear, x, y, 0);
    if (state != Idle) {
        if (currentGear < MAX_GEAR - 1) {
            car->drawAnimationFrame(renderer, CAR_GEARS_AUTO, currentGear + 1,
                                    x, y - GEAR_DISPL_H, 0);
        }
        if (currentGear > 0) {
            car->drawAnimationFrame(renderer, CAR_GEARS_AUTO, currentGear - 1,
                                    x, y + GEAR_DISPL_H, 0);
        }
    }
    renderer->setClip(0, 0, SCREEN_W, SCREEN_H);
}

void GearShiftAuto::onUp() {
    if (state == Idle && currentGear < MAX_GEAR) {
        state = ShiftUp;
    }
}

void GearShiftAuto::onDown() {
    if (state == Idle && currentGear > 0) {
        state = ShiftDown;
    }
}

#define STICK_MOVE_PER_FRAME_H  (2)
#define STICK_MOVE_PER_FRAME_V  (2)
#define STICK_H_STEP            (6)
#define STICK_V_STEP            (5)

void GearShiftManual::reset() {
    currentGear = 0;
    currentX = 1;
    currentY = 1;
    currentOffset = 0;
    targetOffset = 0;
    state = Idle;
}

void GearShiftManual::getNextPosition(ShiftState direction,
                                      int8_t& x, int8_t& y) {
    switch (direction) {
        case Left:  x--; break;
        case Right: x++; break;
        case Up:    y--; break;
        case Down:  y++; break;
    }
}

int8_t GearShiftManual::getGearAtPosition(int8_t x, int8_t y) {
    static constexpr int32_t speedGrid = ((1l << 0)  | (3l << 3)  | (5l << 6)  |
                                          (0l << 9)  | (0l << 12) | (0l << 15) |
                                          (2l << 18) | (4l << 21) | (7l << 24));
    int8_t bitset = ((y * 3) + x) * 3;
    return ((speedGrid >> bitset) & 7);
}

bool GearShiftManual::canGoDirection(ShiftState direction) {
    int8_t newX = currentX;
    int8_t newY = currentY;
    if (currentY != 1 && (direction == Left || direction == Right)) {
        return false;
    }
    getNextPosition(direction, newX, newY);
    if (newX < 0 || newX > 2 || newY < 0 || newY > 2 ||
        getGearAtPosition(newX, newY) == 7) {
            return false;
    }
    return true;
}

void GearShiftManual::tryGoDirection(ShiftState direction) {
    if (state != Idle) return;
    if (canGoDirection(direction)) {
        state = direction;
        targetOffset = (direction == Up || direction == Down) ? STICK_V_STEP :
                        STICK_H_STEP;
    }
}

void GearShiftManual::update() {
    if (state != Idle) {
        int8_t offIncr = (state == Up || state == Down) ?
                          STICK_MOVE_PER_FRAME_V : STICK_MOVE_PER_FRAME_H;
        currentOffset += offIncr;
        if (currentOffset >= targetOffset) {
            currentOffset = 0;
            getNextPosition(state, currentX, currentY);
            state = Idle;
        }
    }
}

void GearShiftManual::draw(SpriteRenderer* renderer, int16_t x, int16_t y) {
    Sprite* car = GetSprite(SPRITE_CAR);
    car->drawAnimationFrame(renderer,
                            CAR_GEARS_MANUAL, HUD_GEARS_A_BG, x, y, 0);
    int16_t xPos = x + (currentX - 1) * STICK_H_STEP;
    int16_t yPos = y + (currentY - 1) * STICK_V_STEP;
    switch (state) {
        case Left:  xPos -= currentOffset; break;
        case Right: xPos += currentOffset; break;
        case Up:    yPos -= currentOffset; break;
        case Down:  yPos += currentOffset; break;
    }
    car->drawAnimationFrame(renderer,
                            CAR_GEARS_MANUAL, HUD_GEARS_A_STICK, xPos, yPos, 0);
}