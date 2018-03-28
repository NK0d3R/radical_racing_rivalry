// Copyright 2018 Catalin G. Manciu

#include "level.h"
#include "renderer.h"
#include "sprites.h"
#include "car.h"
#include "enemycar.h"

int16_t Level::BackgroundLayer::camPosToOffset(const FP32& cameraPosition) {
    return -(M_TO_PIXELS((cameraPosition * offsetFactor) / 1000).getInt());
}

void Level::BackgroundGrid::drawSingleLine(SpriteRenderer* renderer,
                                           int16_t x, int16_t yTop,
                                           int16_t yBot) {
    FP32 lineX(x);
    FP32 lineXBottom = lineX * FP32(3.25f);
    Line current(lineX + Defs::FP_HALF_SCR_W, FP32(yTop),
                 lineXBottom + Defs::FP_HALF_SCR_W, FP32(yBot));
    renderer->getClip().clipLineX(&current);
    if (current.exists()) {
        renderer->drawLine(current.start.x.getInt(),
                           current.start.y.getInt(),
                           current.end.x.getInt(),
                           current.end.y.getInt());
    }
}

void Level::BackgroundGrid::draw(SpriteRenderer* renderer,
                                 const FP32& cameraPosition) {
    int32_t offset = camPosToOffset(cameraPosition);
    offset %= density;
    for (int16_t line_x_top = offset; line_x_top < (SCREEN_W / 2);
        line_x_top += density) {
        drawSingleLine(renderer, line_x_top, yTop, yBot);
    }
    for (int16_t line_x_top = offset - density;
        line_x_top > -(SCREEN_W / 2); line_x_top -= density) {
        drawSingleLine(renderer, line_x_top, yTop, yBot);
    }
    int16_t crt_y = yTop;
    renderer->drawLine(0, crt_y, SCREEN_W - 1, crt_y);
    crt_y = yBot;
    renderer->drawLine(0, crt_y, SCREEN_W - 1, crt_y);

    for (int16_t idx = 2; idx > 0; idx --) {
        crt_y = ((yTop + crt_y) >> 1);
        renderer->drawLine(0, crt_y, SCREEN_W - 1, crt_y);
    }
}


void Level::BackgroundSprite::draw(SpriteRenderer* renderer,
                                   const FP32& cameraPosition) {
    int32_t offset = (SCREEN_W / 2);
    if (offsetFactor != 0) {
        int16_t maxOffset = SCREEN_W + (width >> 1);
        offset += (camPosToOffset(cameraPosition) % width);
        if (offset > 0) {
            offset -= width;
        }
        do {
            GetSprite(SPRITE_ENV)->drawAnimationFrame(renderer,
                                                      BACKGROUND_ANIM,
                                                      frame, offset,
                                                      yPos, 0);
            offset += width;
        } while (offset < maxOffset);
    } else {
            GetSprite(SPRITE_ENV)->drawAnimationFrame(renderer, BACKGROUND_ANIM,
                                                      frame, offset, yPos, 0);
    }
}


Level::BackgroundChopper::BackgroundChopper() : BackgroundLayer(0) {
    chopperAnim.init(GetSprite(SPRITE_ENV));
    xSpeed = FP32(random(100) & 1 ? 1 : -1);
    wait(true);
}

void Level::BackgroundChopper::wait(bool isWaiting) {
    waiting = isWaiting;
    if (waiting) {
        timer = random(BG_CHOPPER_WAIT_TIME_MIN, BG_CHOPPER_WAIT_TIME_MAX);
    } else {
        timer = BG_CHOPPER_DECISION_TIME;
    }
}

void Level::BackgroundChopper::restart() {
    int32_t direction = -SGN(xSpeed.getInt());
    if (direction < 0) {
        xPos = FP32(SCREEN_W + BG_CHOPPER_MARGIN_OFFSET);
    } else {
        xPos = FP32(-BG_CHOPPER_MARGIN_OFFSET);
    }
    xSpeed = FP32(direction * random(20, 40));
    chopperAnim.setAnimation(BACKGROUND_CHOPPER_ANIM,
                            direction == -1 ? ARD_FLAGS_FLIP_X : 0,
                            true);
    yPos = random(10, 15);
}

void Level::BackgroundChopper::update(int16_t dt) {
    if (waiting == false) {
        xPos += (xSpeed * FP32(dt)) / 1000;
        if (xPos.getInt() < -BG_CHOPPER_MARGIN_OFFSET ||
            xPos.getInt() > SCREEN_W + BG_CHOPPER_MARGIN_OFFSET) {
            wait(true);
        } else {
            timer -= dt;
            if (timer < 0) {
                timer = BG_CHOPPER_DECISION_TIME;
                if (random(100) < 60) {
                    yPos += random(-1, 3);
                }
                if (random(100) < 10) {
                    xSpeed *= -1;
                    chopperAnim.setAnimation(BACKGROUND_CHOPPER_ANIM,
                                             xSpeed.getInt() < 0 ?
                                             ARD_FLAGS_FLIP_X : 0,
                                             true);
                }
            }
        }
        chopperAnim.update(dt);
    } else {
        timer -= dt;
        if (timer < 0) {
            wait(false);
            restart();
        }
    }
}

void Level::BackgroundChopper::draw(SpriteRenderer* renderer,
                                   const FP32& cameraPosition) {
    if (waiting == false) {
        chopperAnim.draw(renderer, xPos.getInt(), yPos);
    }
}

Level::BackgroundAnim::BackgroundAnim(int16_t y, int16_t width,
                                      uint8_t animation, int16_t factor):
                                      BackgroundLayer(factor), yPos(y),
                                      width(width) {
    bgAnimator.init(GetSprite(SPRITE_ENV));
    bgAnimator.setAnimation(animation, 0, true);
}

void Level::BackgroundAnim::draw(SpriteRenderer* renderer,
                                 const FP32& cameraPosition) {
    int32_t offset = (SCREEN_W / 2);
    if (offsetFactor != 0) {
        offset -= (camPosToOffset(cameraPosition) % width);
        bgAnimator.draw(renderer, offset + width, yPos);
    }
    bgAnimator.draw(renderer, offset, yPos);
}

void Level::BackgroundAnim::update(int16_t dt) {
    bgAnimator.update(dt);
}

void Level::initialize() {
    bgLayers[0] = new BackgroundSprite(25, 0, BACKGROUND_SUN_FRM, 0);
    bgLayers[1] = new BackgroundSprite(23, 190, BACKGROUND_LAYER_1, 25);
    bgLayers[2] = new BackgroundSprite(25, 190, BACKGROUND_LAYER_1, 100);
    bgLayers[3] = new BackgroundGrid(25, 40, 10, 135);
    bgLayers[4] = new BackgroundChopper();
    bgLayers[5] = new BackgroundSprite(42, 240, BACKGROUND_LAYER_2, 430);

    objectsInventory[PLAYER_CAR] = new Car(this, 0, FP32(0.0f), 43);
    objectsInventory[ENEMY_CAR] = new EnemyCar(this, 0, FP32(0.0f), 43);

    restart();
}

void Level::restart() {
    cameraPosition = 0;
    nbActiveObjects = 0;
    enemyCarIdx = nbActiveObjects;
    activeObjects[nbActiveObjects ++] = objectsInventory[ENEMY_CAR];
    playerCarIdx = nbActiveObjects;
    activeObjects[nbActiveObjects ++] = objectsInventory[PLAYER_CAR];
    static_cast<Car*>(activeObjects[playerCarIdx])->reset(FP32(0.8));
    static_cast<Car*>(activeObjects[playerCarIdx])->updateScreenY();
    static_cast<Car*>(activeObjects[enemyCarIdx])->reset(FP32(0.2));
    static_cast<Car*>(activeObjects[enemyCarIdx])->updateScreenY();
}

void Level::draw(SpriteRenderer* renderer) {
    for (auto layer : bgLayers) {
        layer->draw(renderer, cameraPosition);
    }

    for (uint8_t idx = 0; idx < nbActiveObjects; ++idx) {
        if (activeObjects[idx]->isVisible()) {
            activeObjects[idx]->draw(renderer);
        }
    }
    drawHUD(renderer);
}

#define DEBUG_ENEMY_SPEED   (1)

void Level::drawMainCarHUD(SpriteRenderer* renderer, int16_t x, int16_t y) {
// Draw RPM bar
    Car& mainCar = *static_cast<Car*>(activeObjects[playerCarIdx]);
#if DEBUG_ENEMY_SPEED
    #define NB_SPEED_DIGITS (6)
    Car& enemyCar = *static_cast<Car*>(activeObjects[enemyCarIdx]);
#else
    #define NB_SPEED_DIGITS (3)
#endif
    GetSprite(SPRITE_CAR)->drawAnimationFrame(renderer, CAR_RPM_HUD,
                                              HUD_FRAME_RPM, x, y, 0);
    int32_t barLength = ((mainCar.getRPM() * MAX_RPM_BAR_LENGTH) /
                          Defs::MAX_RPM).getInt();
    CLAMP_UPPER(barLength, MAX_RPM_BAR_LENGTH);
    renderer->setClip(x + 1, 0, barLength + 2, SCREEN_H);
    GetSprite(SPRITE_CAR)->drawAnimationFrame(renderer, CAR_RPM_HUD,
                                              HUD_FRAME_RPM_BAR, x, y, 0);
    renderer->setClip(0, 0, SCREEN_W, SCREEN_H);
// Draw speed
    int16_t crtX = x + 23;
    int16_t crtY = y - 5;
#if DEBUG_ENEMY_SPEED
    int32_t speed = MPS_TO_KPH(mainCar.getSpeed()).getInt() * 1000 +
                     MPS_TO_KPH(enemyCar.getSpeed()).getInt();
#else
    int32_t speed = MPS_TO_KPH(mainCar.getSpeed()).getInt();
#endif
    for (int8_t digit = 0; digit < NB_SPEED_DIGITS; ++digit) {
        GetSprite(SPRITE_CAR)->drawAnimationFrame(renderer, CAR_SPEED_FONT,
                                                  (speed % 10), crtX, crtY, 0);
        speed /= 10;
        crtX -= CAR_SPEED_FONT_W;
    }
// Draw unit
    uint8_t unitFrame = HUD_FRAME_KPH;
    if (mainCar.getRPM() > 7200) {
        unitFrame = HUD_FRAME_WARNING;
    }
    GetSprite(SPRITE_CAR)->drawAnimationFrame(renderer, CAR_RPM_HUD,
                                              unitFrame, x, y, 0);
// Draw gear
    GetSprite(SPRITE_CAR)->drawAnimationFrame(renderer, CAR_SPEED_FONT,
                                              mainCar.getGear(), x, y, 0);
}

void Level::drawHUD(SpriteRenderer* renderer) {
    drawMainCarHUD(renderer, 94, 64);
}

void Level::updateControls(uint8_t buttonsState, uint8_t oldButtonsState) {
    Car& mainCar = *static_cast<Car*>(activeObjects[playerCarIdx]);
    uint8_t changedButtons = (buttonsState ^ oldButtonsState);
    mainCar.pedalToTheMetal(buttonsState & A_BUTTON);
    mainCar.setClutch(buttonsState & B_BUTTON);
    if (mainCar.isClutched()) {
        if ((changedButtons & buttonsState & UP_BUTTON)) {
            mainCar.shiftGear(true);
        }
        if ((changedButtons & buttonsState & DOWN_BUTTON)) {
            mainCar.shiftGear(false);
        }
    }
    uint8_t resetMask = (A_BUTTON | B_BUTTON | LEFT_BUTTON);
    if ((buttonsState & resetMask) == resetMask) {
        restart();
    }
}
void Level::update(int16_t dt,
                   uint8_t buttonsState, uint8_t oldButtonsState) {
    updateControls(buttonsState, oldButtonsState);
    for (uint8_t idx = 0; idx < nbActiveObjects; ++idx) {
        activeObjects[idx]->update(dt);
    }
    updateCamera();
    for (uint8_t idx = 0; idx < nbActiveObjects; ++idx) {
        activeObjects[idx]->updateScreenX();
    }
    for (auto layer : bgLayers) {
        layer->update(dt);
    }
}

int16_t Level::worldToScreenX(const FP32& x, const FP32& y) {
    return (Defs::FP_HALF_SCR_W + M_TO_PIXELS(x - cameraPosition) *
                  (FP32(0.7f) + y * FP32(0.3f))).getInt();
}

int16_t Level::worldToScreenY(const FP32& x, const FP32& y) {
    return LEVEL_ACTION_AREA_TOP +
           (y * (LEVEL_ACTION_AREA_BOT - LEVEL_ACTION_AREA_TOP)).getInt();
}

void Level::updateCamera() {
    Car& mainCar = *static_cast<Car*>(activeObjects[playerCarIdx]);
    cameraPosition += mainCar.getX() + FP32(4);
    cameraPosition /= 2;
}
