#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "ardusprite.h"
#include "car.h"

class SpriteRenderer;
class Sprite;

#define NB_BG_LAYERS    (4)

Level& GetLevel();

class Level {
    public:
        void Initialize();
        void Render(SpriteRenderer* renderer);
        void Update(int16_t dt,
                    uint8_t buttons_state, uint8_t old_buttons_state);
        void UpdateControls(uint8_t buttons_state, uint8_t old_buttons_state);
        int32_t WorldToScreen(int32_t pos);
        void UpdateCamera();
        ~Level();
    private:
        class BackgroundLayer{
            public:
                BackgroundLayer(int16_t factor) : offset_factor(factor) {
                }
                virtual void Draw(SpriteRenderer* renderer,
                                  int32_t cameraPosition) = 0;
            protected:
                int16_t offset_factor;
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
                               int16_t _density, int16_t factor) :
                               BackgroundLayer(factor), y_top(_y_top),
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
                                 int16_t frame, int16_t factor) :
                                 BackgroundLayer(factor), y_pos(y),
                                 width(width), frame(frame) {
                 }
                virtual void Draw(SpriteRenderer* renderer,
                                  int32_t camera_position);
        };
        int32_t camera_pos;
        Car main_car;

        BackgroundLayer* bg_layers[NB_BG_LAYERS];
        void DrawLevelBackground(SpriteRenderer* renderer);
        void DrawHud(SpriteRenderer* renderer);
};

#endif

