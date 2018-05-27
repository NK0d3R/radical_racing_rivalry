  // Copyright 2018 Catalin G. Manciu

#include "src/stdinc.h"
#include "src/defs.h"
#include "src/engine/ardusprite.h"
#include "src/engine/renderer.h"
#include "src/game/level.h"
#include "src/game/menu.h"
#include "src/res/stringmgr.h"
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

uint32_t bestTimes[2] = {
    180000,
    220000
};

FP32 bestDistances[2] = {
    FP32(200),
    FP32(180)
};

uint32_t getTimeRecord(uint8_t gearMode) {
    return bestTimes[gearMode];
}

void updateTimeRecord(uint8_t gearMode, uint32_t newValue) {
    bestTimes[gearMode] = newValue;
    saveSave();
}

const char signature[] = {'N', 'K', 'D', '1'};

void saveSave() {
    EEPROM.put(0, signature);
    EEPROM.put(sizeof(signature), bestTimes);
    EEPROM.put(sizeof(signature) + sizeof(bestTimes), bestDistances);
}

void saveLoad() {
    char sign[4];
    EEPROM.get(0, sign);
    if(memcmp(signature, sign, sizeof(sign)) != 0) {
        saveSave();
    } else {
        EEPROM.get(sizeof(signature), bestTimes);
        EEPROM.get(sizeof(signature) + sizeof(bestTimes), bestDistances);
    }
}

void setAppState(AppState newState) {
    if (appState != newState) {
        switch (newState) {
            case MainMenu:
                menu.set(getMenuData(0), 3, 100, Defs::AnimMenuMain);
                menu.setItemOption(0, level.getGameMode());
                menu.setItemOption(1, level.getGearMode());
                menu.restart();
            break;
            case AfterGameMenu:
                menu.set(getMenuData(1), 2, 80, 0);
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
    saveLoad();
    app.beginNoLogo();
    app.setFrameRate(30);
    app.clear();

    buttonsState = 0;
    oldButtonsState = 0;
    frameCounter = 0;
    GetSprite(Defs::SpriteEnv)->create(ENV_SPRITE_DATA);
    GetSprite(Defs::SpriteCar)->create(CAR_SPRITE_DATA);
    GetSprite(Defs::SpriteMenu)->create(MENU_SPRITE_DATA);
    GetFont(Defs::FontMain)->create(FONT_DATA, mapping, nb_map_elems,
                                    Defs::MainFontSpaceW, Defs::MainFontHeight,
                                    default_frame);
    renderer.initialize(app.getBuffer(), 128);
    renderer.setClip(0, 0, 128, 64);
    setAppState(MainMenu);
    //Serial.begin(9600);
}

void loop() {
    if (!app.nextFrame()) {
        return;
    }

    app.clear();

    oldButtonsState = buttonsState;
    buttonsState = app.buttonsState();

    switch(appState) {
        case MainMenu: {
            menu.updateControls(buttonsState, oldButtonsState);
            menu.draw(&renderer, (Defs::ScreenW >> 1), 0);
            if (menu.getAction() == Defs::MenuActionStart) {
                level.setGameMode(menu.getItemOption(0));
                level.setGearMode(menu.getItemOption(1));
                level.restart();
                setAppState(Ingame);
            }
            int8_t string = Strings::BestTime + menu.getItemOption(0);
            getString(string);
            if (menu.getItemOption(0) == 0) {
                Utils::formatTime(bestTimes[menu.getItemOption(1)],
                                  getStringBuffer() + getStringLen(string));
            }
            GetFont(Defs::FontMain)->drawString(
                    &renderer, getStringBuffer(),
                    Defs::ScreenW / 2, 64, ANCHOR_BOTTOM | ANCHOR_HCENTER, -1);
        } break;
        case Ingame: {
            level.updateControls(buttonsState, oldButtonsState);
            level.update(33);
            level.draw(&renderer);
        } break;
        case AfterGameMenu: {
            menu.updateControls(buttonsState, oldButtonsState);
            switch (menu.getAction()) {
                case Defs::MenuActionRestart:
                    level.restart();
                    setAppState(Ingame);
                break;
                case Defs::MenuActionBackToMM:
                    setAppState(MainMenu);
                break;
            }
            menu.draw(&renderer, (Defs::ScreenW >> 1), 16);
        } break;
    }
    app.display();
    ++frameCounter;
}

