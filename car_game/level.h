#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "defs.h"

class SpriteRenderer;
class Sprite;

class Level {
    public:
        void Initialize(Sprite* sprites);
        void Render(SpriteRenderer* renderer);
        void Update();

    private:
        int32_t gridOffset;
        int16_t gridDensity;
        int32_t layer1Offset;
        Sprite* gameSprites;

        void Draw80sLine(SpriteRenderer* renderer, int16_t x, 
                         int16_t y_top, int16_t y_bottom);
        void Draw80sGrid(SpriteRenderer* renderer, 
                         int16_t y_top, int16_t y_bottom);
        void DrawLevelBackground(SpriteRenderer* renderer);
};


#endif
