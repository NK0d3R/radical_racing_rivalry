#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "defs.h"

class SpriteRenderer;
class Sprite;

#define NB_BG_LAYERS    (4)

class Level {
    public:
        void Initialize();
        void Render(SpriteRenderer* renderer);
        void Update();
        ~Level();
    private:
        class BackgroundLayer{
            public:
                BackgroundLayer(int32_t ratio) : offset_ratio(ratio) {
                }
                virtual void Draw(SpriteRenderer* renderer,
                                  int32_t cameraPosition) = 0;
            protected:
                int32_t offset_ratio;
                int32_t CamPosToOffset(int32_t camera_position);
        };

        class BackgroundGrid : public BackgroundLayer {
            int16_t y_top;
            int16_t y_bot;
            int16_t density;
            void DrawSingleLine(SpriteRenderer* renderer, int16_t x,
                                int16_t y_top, int16_t y_bot);
            public:
                BackgroundGrid(int16_t _y_top, int16_t _y_bot,
                               int16_t _density, int32_t ratio) :
                               BackgroundLayer(ratio), y_top(_y_top),
                               y_bot(_y_bot), density(_density) {
                }
                virtual void Draw(SpriteRenderer* renderer,
                                  int32_t cameraPosition);
        };

         class BackgroundSprite : public BackgroundLayer {
            int16_t y_pos;
            int16_t width;
            int16_t frame;
            public:
                BackgroundSprite(int16_t y, int16_t width,
                                 int16_t frame, int32_t ratio) :
                                 BackgroundLayer(ratio), y_pos(y),
                                 width(width), frame(frame) {
                 }
                virtual void Draw(SpriteRenderer* renderer,
                                  int32_t camera_position);
        };
        int32_t camera_pos;
        BackgroundLayer* bg_layers[NB_BG_LAYERS];
        void DrawLevelBackground(SpriteRenderer* renderer);
};

#endif
