// Copyright 2018 Catalin G. Manciu

#include "level.h"
#include "car.h"
#include "enemycar.h"
#include "../res/stringmgr.h"
#include "../engine/renderer.h"
#include "../res/sprites.h"

Level::BackgroundLayer* Level::bgLayers[] = {
    new BackgroundSprite(25, 0, Defs::BackgroundSun, 0),
    new BackgroundSprite(23, 190, Defs::BackgroundLayer1, 25),
    new BackgroundSprite(25, 190, Defs::BackgroundLayer1, 100),
    new BackgroundGrid(25, 40, 10, 135),
    new BackgroundSprite(42, 240, Defs::BackgroundLayer2, 430)
};

int16_t Level::BackgroundLayer::camPosToOffset(const FP32& cameraPosition) {
    FP32 camPosScaled = (cameraPosition * offsetFactor) / 1000;
    return -(Utils::metersToPixels(camPosScaled).getInt());
}

void Level::BackgroundGrid::drawSingleLine(SpriteRenderer* renderer,
                                           int16_t x, int16_t yTop,
                                           int16_t yBot) {
    FP32 lineX(x);
    FP32 lineXBottom = lineX * FP32(3.25f);
    Line current(lineX + Defs::FPHalfScrW, FP32(yTop),
                 lineXBottom + Defs::FPHalfScrW, FP32(yBot));
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
    int16_t offset = camPosToOffset(cameraPosition);
    offset %= density;
    for (int16_t line_x_top = offset; line_x_top < (Defs::ScreenW / 2);
         line_x_top += density) {
        drawSingleLine(renderer, line_x_top, yTop, yBot);
    }
    for (int16_t line_x_top = offset - density;
         line_x_top > -(Defs::ScreenW / 2); line_x_top -= density) {
        drawSingleLine(renderer, line_x_top, yTop, yBot);
    }
    int8_t mid = ((yTop + yBot) >> 1);
    int8_t values[4] = { yTop, yBot, mid, ((yTop + mid) >> 1) };
    for (auto y : values) {
        renderer->drawLine(0, y, Defs::ScreenW - 1, y);
    }
}


void Level::BackgroundSprite::draw(SpriteRenderer* renderer,
                                   const FP32& cameraPosition) {
    int16_t offset = (Defs::ScreenW / 2);
    if (offsetFactor != 0) {
        int16_t maxOffset = Defs::ScreenW + (width >> 1);
        offset += (camPosToOffset(cameraPosition) % width);
        while (offset > (width >> 1)) {
            offset -= width;
        }
        do {
            GetSprite(Defs::SpriteEnv)->drawAnimationFrame(
                                            renderer, Defs::AnimBackgrounds,
                                            frame, offset, yPos, 0);
            offset += width;
        } while (offset < maxOffset);
    } else {
            GetSprite(Defs::SpriteEnv)->drawAnimationFrame(
                                            renderer, Defs::AnimBackgrounds,
                                            frame, offset, yPos, 0);
    }
}


Level::BackgroundChopper::BackgroundChopper() : BackgroundLayer(0) {
    chopperAnim.init(GetSprite(Defs::SpriteEnv));
    xSpeed = FP32(random(100) & 1 ? 1 : -1);
    wait(true);
}

void Level::BackgroundChopper::wait(bool isWaiting) {
    waiting = isWaiting;
    if (waiting) {
        timer = random(Defs::BgChopperMinWaitTime, Defs::BgChopperMaxWaitTime);
    } else {
        timer = Defs::BgChopperDecisionTime;
    }
}

void Level::BackgroundChopper::restart() {
    int32_t direction = -Utils::sgn(xSpeed.getInt());
    if (direction < 0) {
        xPos = FP32(Defs::ScreenW + Defs::BgChopperMarginOffset);
    } else {
        xPos = FP32(-Defs::BgChopperMarginOffset);
    }
    xSpeed = FP32(direction * random(20, 40));
    chopperAnim.setAnimation(Defs::AnimChopper,
                             direction == -1 ? ARD_FLAGS_FLIP_X : 0,
                             true);
    yPos = random(10, 15);
}

void Level::BackgroundChopper::update(int16_t dt) {
    if (waiting == false) {
        xPos += (xSpeed * FP32(dt)) / 1000;
        if (xPos.getInt() < -Defs::BgChopperMarginOffset ||
            xPos.getInt() > Defs::ScreenW + Defs::BgChopperMarginOffset) {
            wait(true);
        } else {
            timer -= dt;
            if (timer < 0) {
                timer = Defs::BgChopperDecisionTime;
                if (random(100) < 60) {
                    yPos += random(-1, 3);
                }
                if (random(100) < 10) {
                    xSpeed *= -1;
                    chopperAnim.setAnimation(Defs::AnimChopper,
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

Level::Level() {
    playerCar = new Car(this, 0, FP32(0), 43);
    enemyCar = new EnemyCar(this, 0, FP32(0), 43);
    screenAnim.init(GetSprite(Defs::SpriteCar));
    state = Invalid;
}

void Level::restart() {
    playerCar->reset(mode == Duel ? FP32(0.9f) : FP32(0.6f));
    playerCar->updateScreenY();
    if (mode == Duel) {
        enemyCar->reset(FP32(0.3f));
        enemyCar->updateScreenY();
    }
    levelTimer = 0;
    currentGearShift->reset();
    setState(Countdown);
}

void Level::startScreenAnim(int8_t x, int8_t y, uint8_t anim) {
    scrAnimX = x;
    scrAnimY = y;
    showScreenAnim = true;
    screenAnim.setAnimation(anim, 0, false);
}

void Level::setState(LevelState newState) {
    if (newState != state) {
        switch (newState) {
            case Countdown:
                startScreenAnim(Defs::ScreenW / 2, Defs::ScreenH / 2,
                                Defs::AnimCarCountdown);
                cameraPosition = -200;
            break;
            case PlayerDead:
                startScreenAnim(120, 62,
                                Defs::AnimCarExplosion);
                stateCounter = 0;
            break;
        }
        state = newState;
    }
}

void Level::foreachGameObject(auto func) {
    if (mode == Duel) {
        func(enemyCar);
    }
    func(playerCar);
}

void Level::draw(SpriteRenderer* renderer) {
    for (auto layer : bgLayers) {
        layer->draw(renderer, cameraPosition);
    }
    drawGameMarkers(renderer);
    foreachGameObject([&](GameObject* obj) {
                       if (obj->isVisible()) obj->draw(renderer);
                      });

    if (state == Race) {
        drawHUD(renderer);
    }

    if (showScreenAnim) {
        screenAnim.draw(renderer, scrAnimX, scrAnimY);
    }
}

#define DEBUG_ENEMY_SPEED   (0)

void Level::drawCarHUD(SpriteRenderer* renderer, int16_t x, int16_t y) {
// Draw RPM bar
#if DEBUG_ENEMY_SPEED
    #define NB_SPEED_DIGITS (6)
#else
    #define NB_SPEED_DIGITS (3)
#endif
    GetSprite(Defs::SpriteCar)->drawAnimationFrame(
                                    renderer, Defs::AnimCarRPMHud,
                                    Defs::HUDFrameRPM, x, y, 0);
    int16_t barLength = ((playerCar->getRPM() * Defs::RPMBarLength) /
                          Defs::MaxRPM).getInt();
    barLength = Utils::upperClamp(barLength, Defs::RPMBarLength);
    renderer->setClip(x + 1, 0, barLength + 2, Defs::ScreenH);
    GetSprite(Defs::SpriteCar)->drawAnimationFrame(
                                    renderer, Defs::AnimCarRPMHud,
                                    Defs::HUDFrameRPMBar, x, y, 0);
    renderer->setClip(0, 0, Defs::ScreenW, Defs::ScreenH);
// Draw speed
    int16_t crtX = x + 23;
    int16_t crtY = y - 5;
#if DEBUG_ENEMY_SPEED
    int32_t speed = Utils::mpsToKph(playerCar->getSpeed()).getInt() * 1000 +
                    Utils::mpsToKph(enemyCar->getSpeed()).getInt();
#else
    int32_t speed = Utils::mpsToKph(playerCar->getSpeed()).getInt();
#endif
    for (int8_t digit = 0; digit < NB_SPEED_DIGITS; ++digit) {
        GetSprite(Defs::SpriteCar)->drawAnimationFrame(
                                        renderer, Defs::AnimCarSpeedFont,
                                        (speed % 10), crtX, crtY, 0);
        speed /= 10;
        crtX -= Defs::CarSpeedFontW;
    }
// Draw unit
    uint8_t unitFrame = Defs::HUDFrameKPH;
    if (playerCar->getRPM() > 7200) {
        unitFrame = Defs::HUDFrameWarning;
    }
    GetSprite(Defs::SpriteCar)->drawAnimationFrame(
                                    renderer, Defs::AnimCarRPMHud,
                                    unitFrame, x, y, 0);
// Draw gear
    GetSprite(Defs::SpriteCar)->drawAnimationFrame(
                                    renderer, Defs::AnimCarSpeedFont,
                                    playerCar->getGear(), x, y, 0);
}

void Level::drawHUD(SpriteRenderer* renderer) {
    if (playerCar->isClutched()) {
        currentGearShift->draw(renderer, 110, 52);
    } else {
        drawCarHUD(renderer, 94, 64);
    }
    if (mode == TimeAttack) {
        drawTimer(renderer, 0, 64);
    }
}

void Level::drawTimer(SpriteRenderer* renderer, int16_t x, int16_t y) {
    char* timerStr = getStringBuffer();
    uint16_t sec = levelTimer / 1000;
    uint16_t msec = levelTimer - (sec * 1000);
    uint16_t min = sec / 60;
    sec -= min * 60;
    timerStr[9] = 0;
    Utils::fastGetDigits(msec, &timerStr[8], 3);
    Utils::fastGetDigits(sec, &timerStr[4], 2);
    Utils::fastGetDigits(min, &timerStr[1], 2);
    timerStr[5] = '.';
    timerStr[2] = ':';
    GetFont(Defs::FontMain)->drawString(renderer, timerStr, x, y,
                                        ANCHOR_LEFT | ANCHOR_BOTTOM);
}

void Level::updateControls(uint8_t buttonsState, uint8_t oldButtonsState) {
    uint8_t changedButtons = (buttonsState ^ oldButtonsState);
    if (state != Race) {
        return;
    }
    playerCar->pedalToTheMetal(buttonsState & A_BUTTON);
    playerCar->setClutch(buttonsState & B_BUTTON);
    if (playerCar->isClutched()) {
        if ((changedButtons & buttonsState & UP_BUTTON)) {
            currentGearShift->onUp();
        }
        if ((changedButtons & buttonsState & DOWN_BUTTON)) {
            currentGearShift->onDown();
        }
        if ((changedButtons & buttonsState & LEFT_BUTTON)) {
            currentGearShift->onLeft();
        }
        if ((changedButtons & buttonsState & RIGHT_BUTTON)) {
            currentGearShift->onRight();
        }
    } else if (changedButtons & B_BUTTON) {
        int8_t shiftResult = currentGearShift->getShiftResult();
        if (shiftResult != -1) {
            playerCar->shiftGear(shiftResult);
        } else {
            playerCar->destroy();
        }
    }
    uint8_t resetMask = (A_BUTTON | B_BUTTON | LEFT_BUTTON);
    if ((buttonsState & resetMask) == resetMask) {
        restart();
    }
}

void Level::update(int16_t dt) {
    updateState(dt);
    updateGeneral(dt);
}

void Level::updateState(int16_t dt) {
    switch (state) {
        case Countdown:
            if (screenAnim.animPlaying() == false) {
                raceStart();
            }
        break;
        case Race:
            if (playerCar->isAlive() == false) {
                setState(PlayerDead);
                break;
            }
            if (playerCar->isClutched()) {
                currentGearShift->update();
            }
            if (showScreenAnim && playerCar->getSpeed() > 0) {
                showScreenAnim = false;
            }
            levelTimer += dt;
            if (levelTimer > 1000000000) {
                levelTimer = 1000000000;
            }
        break;
        case PlayerDead:
            if (screenAnim.animPlaying() == false) {
                showScreenAnim = false;
            }
            stateCounter++;
            if (stateCounter > 120) {
                setAppState(AfterGameMenu);
                setState(Invalid);
            }
        break;
    }
}

void Level::raceStart() {
    foreachGameObject([&](GameObject* obj) { obj->onRaceStart(); });
    setState(Race);
}

void Level::raceEnd() {
    foreachGameObject([&](GameObject* obj) { obj->onRaceEnd(); });
}

void Level::updateGeneral(int16_t dt) {
    if (showScreenAnim) {
        screenAnim.update(dt);
    }
    foreachGameObject([&](GameObject* obj) { obj->update(dt); });
    updateCamera();
    foreachGameObject([&](GameObject* obj) { obj->updateScreenX(); });
    for (auto layer : bgLayers) {
        layer->update(dt);
    }
}

void Level::drawMarker(SpriteRenderer* renderer, const FP32& worldPos) {
    FP32 x1(worldToScreenX(worldPos, FP32(0)));
    FP32 y1(worldToScreenY(worldPos, FP32(0)));
    FP32 x2(worldToScreenX(worldPos, FP32(1)));
    FP32 y2(worldToScreenY(worldPos, FP32(1)));
    Line current(x1, y1, x2, y2);
    renderer->getClip().clipLineX(&current);
    if (current.exists()) {
        renderer->drawLine(current.start.x.getInt(),
                           current.start.y.getInt(),
                           current.end.x.getInt(),
                           current.end.y.getInt());
    }
}

void Level::drawGameMarkers(SpriteRenderer* renderer) {
    drawMarker(renderer, FP32(0));
    drawMarker(renderer, Defs::RaceLength);
}

int32_t Level::worldToScreenX(const FP32& x, const FP32& y) {
    return (Defs::FPHalfScrW + Utils::metersToPixels(x - cameraPosition) *
            (FP32(0.7f) + y * FP32(0.5f))).getInt();
}

int16_t Level::worldToScreenY(const FP32& x, const FP32& y) {
    return Defs::LevelActionAreaTop + (y * (Defs::LevelActionAreaBottom -
                                            Defs::LevelActionAreaTop)).getInt();
}

void Level::updateCamera() {
    cameraPosition += playerCar->getX() + FP32(1.5f);
    cameraPosition /= 2;
}
