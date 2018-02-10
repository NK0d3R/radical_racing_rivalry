
#include "defs.h"
#include "ardusprite.h"
#include "renderer.h"
#include "level.h"
#include "env_sprite.h"

Arduboy app;
uint8_t         buttons_state;
uint8_t         old_buttons_state;
Sprite          sprites[NB_SPRITES];
SpriteRenderer  renderer;
Level           lvl;

void setup() {
    app.begin();
    app.setFrameRate(30);
    app.clear();

    buttons_state = 0;
    old_buttons_state = 0;

    CreateSprite(&sprites[SPRITE_ENV], ENV_SPRITE_DATA);
    
    renderer.Initialize(app.getBuffer(), 128);
    renderer.SetClip(0, 0, 128, 64);
    lvl.Initialize(sprites);
}

void loop() {
    if(!app.nextFrame()) {
        return;
    }

    app.clear();

    old_buttons_state = buttons_state;
    buttons_state = app.buttonsState();

    lvl.Update();
    lvl.Render(&renderer);

    app.display();
}
