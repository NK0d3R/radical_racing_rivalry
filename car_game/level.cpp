#include "level.h"
#include "renderer.h"
#include "sprites.h"
#include "defs.h"

void Level::Initialize() {
    camera_pos = 0;
    bg_layers[0] = new BackgroundSprite(25, 0, BACKGROUND_SUN_FRM, 0);
    bg_layers[1] = new BackgroundSprite(23, 190, BACKGROUND_LAYER_1,
                                        25);
    bg_layers[2] = new BackgroundSprite(25, 190, BACKGROUND_LAYER_1,
                                        100);
    bg_layers[3] = new BackgroundGrid(25, 40, 15, 135);
    main_car.Initialize(60);
}

void Level::Render(SpriteRenderer* renderer) {
    DrawLevelBackground(renderer);
    main_car.Draw(renderer);
    DrawHud(renderer);
}

int32_t Level::BackgroundLayer::CamPosToOffset(int32_t camera_position) {
    return FPTOI(M_TO_PIXELS((camera_position * offset_factor) / 10));
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

    for(int16_t idx = 2; idx > 0; idx --) {
        crt_y = ((y_top + crt_y) >> 1);
        renderer->DrawLine(Line(0, crt_y, SCREEN_W - 1, crt_y));
    }
}

void Level::BackgroundSprite::Draw(SpriteRenderer* renderer,
                                   int32_t camera_position) {
    int32_t offset = (SCREEN_W / 2);
    if(offset_factor != 0) {
        offset -= (CamPosToOffset(camera_position) % width);
        GetSprite(SPRITE_ENV)->DrawAnimationFrame(renderer,
                                          BACKGROUND_ANIM,
                                          frame, offset + width,
                                          y_pos, 0);
    }
    GetSprite(SPRITE_ENV)->DrawAnimationFrame(renderer,
                                              BACKGROUND_ANIM,
                                              frame, offset, y_pos,
                                              0);
}


void Level::DrawLevelBackground(SpriteRenderer* renderer) {
    for(int idx = 0; idx < NB_BG_LAYERS; ++ idx) {
        bg_layers[idx]->Draw(renderer, camera_pos);
    }
}

void Level::DrawHud(SpriteRenderer* renderer) {
    char tmp[32];
    sprintf(tmp, "Gear:%d", main_car.GetGear());
    GetFont(FONT_MAIN)->DrawString(renderer, tmp, SCREEN_W, 45,
                                   (ANCHOR_BOTTOM | ANCHOR_RIGHT));
    sprintf(tmp, "%dRPM", FPTOI(main_car.GetRPM()));
    GetFont(FONT_MAIN)->DrawString(renderer, tmp, SCREEN_W, 55,
                                   (ANCHOR_BOTTOM | ANCHOR_RIGHT));
    sprintf(tmp, "%dKPH", FPTOI(MPS_TO_KPH(main_car.GetSpeed())));
    GetFont(FONT_MAIN)->DrawString(renderer, tmp, SCREEN_W, 64,
                                   (ANCHOR_BOTTOM | ANCHOR_RIGHT));
}

void Level::UpdateControls(uint8_t buttons_state, uint8_t old_buttons_state) {
    uint8_t just_changed_buttons = (buttons_state ^ old_buttons_state);
    main_car.PedalToTheMetal(buttons_state & A_BUTTON);
    main_car.Clutch(buttons_state & B_BUTTON);
    if(main_car.IsClutched()){
        if((just_changed_buttons & buttons_state & UP_BUTTON)) {
            main_car.ShiftGear(true);
        }
        if((just_changed_buttons & buttons_state & DOWN_BUTTON)) {
            main_car.ShiftGear(false);
        }
    }
}
void Level::Update(int16_t dt,
                   uint8_t buttons_state, uint8_t old_buttons_state) {
    UpdateControls(buttons_state, old_buttons_state);
    main_car.Update(dt);
    UpdateCamera();
}

int32_t Level::WorldToScreen(int32_t pos) {
    return (SCREEN_W / 2) + FPTOI(M_TO_PIXELS(pos - camera_pos)) - 20;
}

void Level::UpdateCamera() {
    camera_pos = ((camera_pos + (main_car.GetX()<<2)) / 5);
}

Level::~Level() {
    for(int idx = 0; idx < NB_BG_LAYERS; ++ idx) {
        delete bg_layers[idx];
    }
}

