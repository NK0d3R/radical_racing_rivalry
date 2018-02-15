#include "sprites.h"
#include "ardusprite.h"

Sprite sprites[NB_SPRITES];
Font fonts[NB_FONTS];

Sprite* GetSprite(int8_t idx) {
    return &sprites[idx];
}

Font* GetFont(int8_t idx) {
    return &fonts[idx];
}

