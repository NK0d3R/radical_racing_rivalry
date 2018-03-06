// Copyright 2018 Catalin G. Manciu

#include "stdinc.h"
#include "defs.h"
#include "ardusprite.h"
#include "renderer.h"
#include "level.h"
#include "sprites.h"
#include "env_sprite.h"
#include "car_sprite.h"
#include "font.h"
#include "fontmap.h"

Arduboy app;
uint8_t         buttonsState;
uint8_t         oldButtonsState;
SpriteRenderer  renderer;
Level           lvl;

Level& GetLevel() {
    return lvl;
}

void setup() {
    app.begin();
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

    lvl.update(33, buttonsState, oldButtonsState);
    lvl.render(&renderer);

    app.display();
}

