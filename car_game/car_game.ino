// Copyright 2018 Catalin G. Manciu

#include "src/stdinc.h"
#include "src/defs.h"
#include "src/engine/ardusprite.h"
#include "src/engine/renderer.h"
#include "src/game/level.h"
#include "src/game/menu.h"
#include "src/res/sprites.h"
#include "src/res/env_sprite.h"
#include "src/res/car_sprite.h"
#include "src/res/menu_sprite.h"
#include "src/res/font.h"
#include "src/res/fontmap.h"

Arduboy app;
uint8_t         buttonsState;
uint8_t         oldButtonsState;
SpriteRenderer  renderer;
Menu            menu;
Level           level;
int32_t         frameCounter;

AppState appState = Invalid;

void setAppState(AppState newState) {
    if (appState != newState) {
        switch (newState) {
            case MainMenu:
                menu.set(getMenuData(0), 3, 100, MENU_MAIN_ANIM);
                menu.setItemOption(0, level.getGameMode());
                menu.setItemOption(0, level.getGearMode());
                menu.restart();
            break;
        }
        appState = newState;
    }
}

int32_t getFrameCounter() {
    return frameCounter;
}

void setup() {
    app.beginNoLogo();
    app.setFrameRate(30);
    app.clear();

    buttonsState = 0;
    oldButtonsState = 0;
    frameCounter = 0;
    GetSprite(SPRITE_ENV)->create(ENV_SPRITE_DATA);
    GetSprite(SPRITE_CAR)->create(CAR_SPRITE_DATA);
    GetSprite(SPRITE_MENU)->create(MENU_SPRITE_DATA);
    GetFont(FONT_MAIN)->create(FONT_DATA, mapping, nb_map_elems,
                               MAIN_FONT_SPACE_W, MAIN_FONT_HEIGHT,
                               default_frame);
    renderer.initialize(app.getBuffer(), 128);
    renderer.setClip(0, 0, 128, 64);
    setAppState(MainMenu);
}

void loop() {
    if (!app.nextFrame()) {
        return;
    }

    app.clear();

    oldButtonsState = buttonsState;
    buttonsState = app.buttonsState();

    switch(appState) {
        case MainMenu:
            menu.updateControls(buttonsState, oldButtonsState);
            menu.draw(&renderer, (SCREEN_W >> 1), 0);
            if (menu.getAction() == ACTION_START_GAME) {
                level.setGameMode(menu.getItemOption(0));
                level.setGearMode(menu.getItemOption(1));
                level.restart();
                setAppState(Ingame);
            }
        break;
        case Ingame:
            level.updateControls(buttonsState, oldButtonsState);
            level.update(33);
            level.draw(&renderer);
        break;
    }
    app.display();
    ++frameCounter;
}

