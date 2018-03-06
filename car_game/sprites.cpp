// Copyright 2018 Catalin G. Manciu

#include "sprites.h"
#include "ardusprite.h"
#include "defs.h"

Sprite sprites[NB_SPRITES];
Font fonts[NB_FONTS];

Sprite* GetSprite(int8_t idx) {
    return &sprites[idx];
}

Font* GetFont(int8_t idx) {
    return &fonts[idx];
}

