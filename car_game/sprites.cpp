#include "sprites.h"
#include "ardusprite.h"

Sprite sprites[NB_SPRITES];

Sprite* GetSprite(int8_t idx) {
    return &sprites[idx];
}

