// Copyright 2018 Catalin G. Manciu

#include "src/stdinc.h"
#include "src/defs.h"
#include "src/engine/ardusprite.h"
#include "src/engine/renderer.h"
#include "src/game/level.h"
#include "src/res/sprites.h"
#include "src/res/env_sprite.h"
#include "src/res/car_sprite.h"
#include "src/res/font.h"
#include "src/res/fontmap.h"

Arduboy app;
uint8_t         buttonsState;
uint8_t         oldButtonsState;
SpriteRenderer  renderer;
Level           lvl;

Level& GetLevel() {
    return lvl;
}

void setup() {
    app.beginNoLogo();
    app.setFrameRate(30);
    app.clear();

    buttonsState = 0;
    oldButtonsState = 0;
    GetSprite(SPRITE_ENV)->create(ENV_SPRITE_DATA);
    GetSprite(SPRITE_CAR)->create(CAR_SPRITE_DATA);
    GetFont(FONT_MAIN)->create(FONT_DATA, mapping, nb_map_elems,
                               MAIN_FONT_SPACE_W, MAIN_FONT_HEIGHT,
                               default_frame);
    renderer.initialize(app.getBuffer(), 128);
    renderer.setClip(0, 0, 128, 64);
    lvl.initialize();
    Serial.begin(9600);
}

void loop() {
    if (!app.nextFrame()) {
        return;
    }

    app.clear();

    oldButtonsState = buttonsState;
    buttonsState = app.buttonsState();

    lvl.updateControls(buttonsState, oldButtonsState);
    lvl.update(33);
    lvl.draw(&renderer);
    app.display();
}

