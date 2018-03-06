// Copyright 2018 Catalin G. Manciu

#include "level.h"
#include "renderer.h"
#include "sprites.h"

void Level::initialize() {
    cameraPosition = 0;
    bgLayers[0] = new BackgroundSprite(25, 0, BACKGROUND_SUN_FRM, 0);
    bgLayers[1] = new BackgroundSprite(23, 190, BACKGROUND_LAYER_1, 25);
    bgLayers[2] = new BackgroundSprite(25, 190, BACKGROUND_LAYER_1, 100);
    bgLayers[3] = new BackgroundGrid(25, 40, 10, 135);
    mainCar.initialize(60);
}

void Level::render(SpriteRenderer* renderer) {
    drawLevelBackground(renderer);
    mainCar.draw(renderer);
    drawHud(renderer);
}

int16_t Level::BackgroundLayer::camPosToOffset(const FP32& cameraPosition) {
    return (M_TO_PIXELS((cameraPosition * offsetFactor) / 10).getInt());
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
    for (int16_t line_x_top = -offset; line_x_top < (SCREEN_W / 2);
        line_x_top += density) {
        drawSingleLine(renderer, line_x_top, yTop, yBot);
    }
    for (int16_t line_x_top = -offset - density;
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
        offset -= (camPosToOffset(cameraPosition) % width);
        GetSprite(SPRITE_ENV)->drawAnimationFrame(renderer,
                                                  BACKGROUND_ANIM,
                                                  frame, offset + width,
                                                  yPos, 0);
    }
    GetSprite(SPRITE_ENV)->drawAnimationFrame(renderer,
                                              BACKGROUND_ANIM,
                                              frame, offset, yPos,
                                              0);
}


void Level::drawLevelBackground(SpriteRenderer* renderer) {
    for (int idx = 0; idx < NB_BG_LAYERS; ++idx) {
        bgLayers[idx]->draw(renderer, cameraPosition);
    }
}

void Level::drawHud(SpriteRenderer* renderer) {
    char tmp[32];
    snprintf(tmp, sizeof(tmp), "Gear:%d", mainCar.getGear());
    GetFont(FONT_MAIN)->drawString(renderer, tmp, SCREEN_W, 45,
                                   (ANCHOR_BOTTOM | ANCHOR_RIGHT));
    snprintf(tmp, sizeof(tmp), "%dRPM", mainCar.getRPM().getInt());
    GetFont(FONT_MAIN)->drawString(renderer, tmp, SCREEN_W, 55,
                                   (ANCHOR_BOTTOM | ANCHOR_RIGHT));
    snprintf(tmp, sizeof(tmp), "%dKPH",
             MPS_TO_KPH(mainCar.getSpeed()).getInt());
    GetFont(FONT_MAIN)->drawString(renderer, tmp, SCREEN_W, 64,
                                   (ANCHOR_BOTTOM | ANCHOR_RIGHT));
}

void Level::updateControls(uint8_t buttonsState, uint8_t oldButtonsState) {
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
}
void Level::update(int16_t dt,
                   uint8_t buttonsState, uint8_t oldButtonsState) {
    updateControls(buttonsState, oldButtonsState);
    mainCar.update(dt);
    updateCamera();
}

int32_t Level::worldToScreen(const FP32& pos) {
    FP32 offset = M_TO_PIXELS(pos - cameraPosition);
    return (SCREEN_W / 2) + offset.getInt() - 20;
}

void Level::updateCamera() {
    cameraPosition += mainCar.getX();
    cameraPosition /= 2;
}

Level::~Level() {
    for (int idx = 0; idx < NB_BG_LAYERS; ++idx) {
        delete bgLayers[idx];
    }
}

