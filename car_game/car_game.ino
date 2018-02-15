
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
uint8_t         buttons_state;
uint8_t         old_buttons_state;
SpriteRenderer  renderer;
Level           lvl;

Level& GetLevel() {
    return lvl;
}

void setup() {
    app.begin();
    app.setFrameRate(30);
    app.clear();

    buttons_state = 0;
    old_buttons_state = 0;
    GetSprite(SPRITE_ENV)->Create(ENV_SPRITE_DATA);
    GetSprite(SPRITE_CAR)->Create(CAR_SPRITE_DATA);
    GetFont(FONT_MAIN)->Create(FONT_DATA, mapping, nb_map_elems,
                               MAIN_FONT_SPACE_W, MAIN_FONT_HEIGHT,
                               default_frame);
    renderer.Initialize(app.getBuffer(), 128);
    renderer.SetClip(0, 0, 128, 64);
    lvl.Initialize();
    Serial.begin(9600);
}

void loop() {
    if(!app.nextFrame()) {
        return;
    }

    app.clear();

    old_buttons_state = buttons_state;
    buttons_state = app.buttonsState();

    lvl.Update(33, buttons_state, old_buttons_state);
    lvl.Render(&renderer);

    app.display();
}

