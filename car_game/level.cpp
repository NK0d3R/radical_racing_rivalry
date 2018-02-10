#include "level.h"
#include "renderer.h"

#define SPR_ENV &gameSprites[SPRITE_ENV]

void Level::Initialize(Sprite* sprites) {
    gridOffset = 0;
    gridDensity = 16;
    gameSprites = sprites;
}

void Level::Render(SpriteRenderer* renderer) {
    DrawLevelBackground(renderer);
}

void Level::Draw80sLine(SpriteRenderer* renderer, int16_t x, 
                        int16_t y_top, int16_t y_bottom) {
    int32_t line_x = TO_FIXED_POINT(x);
    int32_t line_x_bottom = FROM_FIXED_POINT(line_x * FLOAT_TO_FIXED(3));
    Line current(x + (SCREEN_W / 2), y_top,
                 FROM_FIXED_POINT(line_x_bottom) + (SCREEN_W / 2), y_bottom);
    renderer->ClipLineX(current);
    if(current.Exists()) {
        renderer->DrawLine(current);
    }                        
}

void Level::Draw80sGrid(SpriteRenderer* renderer,
                        int16_t y_top, int16_t y_bottom) {
    for(int32_t line_x_top = -gridOffset; line_x_top < (SCREEN_W / 2); line_x_top += gridDensity) {
        Draw80sLine(renderer, line_x_top, y_top, y_bottom);
    }
    for(int32_t line_x_top = -gridOffset - gridDensity; line_x_top > -(SCREEN_W / 2); line_x_top -= gridDensity) {
        Draw80sLine(renderer, line_x_top, y_top, y_bottom);
    }
    int16_t crt_y = y_top;
    renderer->DrawLine(Line(0, crt_y, SCREEN_W - 1, crt_y));
    crt_y = y_bottom;
    renderer->DrawLine(Line(0, crt_y, SCREEN_W - 1, crt_y));

    for(int16_t idx = 2; idx > 0; idx --) {  
        crt_y = ((y_top + crt_y) >> 1) - idx;
        renderer->DrawLine(Line(0, crt_y, SCREEN_W - 1, crt_y));
    }
}

#define LAYER_1_FRAME_W     190

void Level::DrawLevelBackground(SpriteRenderer* renderer) {
    renderer->DrawAnimationFrame(SPR_ENV, BACKGROUND_ANIM, BACKGROUND_SUN_FRM, (SCREEN_W / 2), 23, 0); 
    Draw80sGrid(renderer, 23, 40);
    renderer->DrawAnimationFrame(SPR_ENV, BACKGROUND_ANIM, BACKGROUND_LAYER_1, (SCREEN_W / 2) - layer1Offset, 23, 0); 
    renderer->DrawAnimationFrame(SPR_ENV, BACKGROUND_ANIM, BACKGROUND_LAYER_1, LAYER_1_FRAME_W + (SCREEN_W / 2) - layer1Offset, 23, 0); 
}

void Level::Update() {
    gridOffset += 1;
    layer1Offset += 1;
    if(gridOffset > gridDensity) {
        gridOffset -= gridDensity;
    }

    if(layer1Offset >= LAYER_1_FRAME_W) {
        layer1Offset -= LAYER_1_FRAME_W;
    }
}

