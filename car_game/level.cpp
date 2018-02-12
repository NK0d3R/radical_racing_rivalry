#include "level.h"
#include "renderer.h"
#include "sprites.h"

void Level::Initialize() {
    camera_pos = 0;
    bg_layers[0] = new BackgroundSprite(25, 0, BACKGROUND_SUN_FRM, 0);
    bg_layers[1] = new BackgroundSprite(23, 190, BACKGROUND_LAYER_1,
                                        FLOAT_TO_FP(0.1));
    bg_layers[2] = new BackgroundSprite(25, 190, BACKGROUND_LAYER_1,
                                        FLOAT_TO_FP(1.0));
    bg_layers[3] = new BackgroundGrid(25, 40, 15, FLOAT_TO_FP(1.10));
}

void Level::Render(SpriteRenderer* renderer) {
    DrawLevelBackground(renderer);
}

int32_t Level::BackgroundLayer::CamPosToOffset(int32_t camera_position) {
    return FPTOI(FPTOI(CM_TO_PIXELS(camera_position) * offset_ratio));
}

void Level::BackgroundGrid::DrawSingleLine(SpriteRenderer* renderer,
                                           int16_t x, int16_t y_top,
                                           int16_t y_bot) {
    int32_t line_x = FP(x);
    int32_t line_x_bottom = FPTOI(line_x * FLOAT_TO_FP(3.5));
    Line current(x + (SCREEN_W / 2), y_top,
                 FPTOI(line_x_bottom) + (SCREEN_W / 2), y_bot);
    renderer->ClipLineX(current);
    if(current.Exists()) {
        renderer->DrawLine(current);
    }                        
}

void Level::BackgroundGrid::Draw(SpriteRenderer* renderer,
                                  int32_t camera_position) {
    int32_t offset = CamPosToOffset(camera_position);
    offset %= density;
    for(int16_t line_x_top = -offset; line_x_top < (SCREEN_W / 2);
            line_x_top += density) {
        DrawSingleLine(renderer, line_x_top, y_top, y_bot);
    }
    for(int16_t line_x_top = -offset - density;
            line_x_top > -(SCREEN_W / 2); line_x_top -= density) {
        DrawSingleLine(renderer, line_x_top, y_top, y_bot);
    }
    int16_t crt_y = y_top;
    renderer->DrawLine(Line(0, crt_y, SCREEN_W - 1, crt_y));
    crt_y = y_bot;
    renderer->DrawLine(Line(0, crt_y, SCREEN_W - 1, crt_y));

    for(int16_t idx = 1; idx >= 0; idx --) {
        crt_y = ((y_top + crt_y) >> 1) - idx;
        renderer->DrawLine(Line(0, crt_y, SCREEN_W - 1, crt_y));
    }
}

void Level::BackgroundSprite::Draw(SpriteRenderer* renderer,
                                   int32_t camera_position) {
    int32_t offset = (SCREEN_W / 2);
    if(offset_ratio != 0) {
        offset -= (CamPosToOffset(camera_position) % width);
    }
    renderer->DrawAnimationFrame(GetSprite(SPRITE_ENV),
                                 BACKGROUND_ANIM,
                                 frame, offset, y_pos,
                                 0);
    if(offset_ratio != 0) {
          renderer->DrawAnimationFrame(GetSprite(SPRITE_ENV),
                                 BACKGROUND_ANIM,
                                 frame, offset + width, y_pos,
                                 0);
    }
}


void Level::DrawLevelBackground(SpriteRenderer* renderer) {
    for(int idx = 0; idx < NB_BG_LAYERS; ++ idx) {
        bg_layers[idx]->Draw(renderer, camera_pos);
    }
}

void Level::Update() {
    camera_pos += 1;
}

Level::~Level() {
    for(int idx = 0; idx < NB_BG_LAYERS; ++ idx) {
        delete bg_layers[idx];
    }
}

