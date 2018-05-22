// Copyright 2018 Catalin G. Manciu

#include "sprites.h"
#include "../engine/ardusprite.h"
#include "../defs.h"

Sprite sprites[Defs::SpritesNb];
Font fonts[Defs::FontsNb];

Sprite* GetSprite(int8_t idx) {
    return &sprites[idx];
}

Font* GetFont(int8_t idx) {
    return &fonts[idx];
}

