// Copyright 2018 Catalin G. Manciu

#ifndef LEVEL_H_
#define LEVEL_H_

#include "stdinc.h"
#include "ardusprite.h"
#include "car.h"
#include "defs.h"

class SpriteRenderer;
class Sprite;

#define NB_BG_LAYERS    (5)

Level& GetLevel();

class Level {
 public:
    void initialize();
    void render(SpriteRenderer* renderer);
    void update(int16_t dt,
                uint8_t buttonsState, uint8_t oldButtonsState);
    void updateControls(uint8_t buttonsState, uint8_t oldButtonsState);
    int32_t worldToScreen(const FP32& pos);
    void updateCamera();

 private:
    class BackgroundLayer {
     public:
        explicit BackgroundLayer(int16_t factor) : offsetFactor(factor) {
        }
        virtual void draw(SpriteRenderer* renderer,
                            const FP32& cameraPosition) = 0;
        virtual void update(int16_t dt) {}
     protected:
        int16_t offsetFactor;
        int16_t camPosToOffset(const FP32& cameraPosition);
    };

    class BackgroundGrid : public BackgroundLayer {
        int16_t yTop;
        int16_t yBot;
        int16_t density;
        void drawSingleLine(SpriteRenderer* renderer, int16_t x,
                            int16_t yTop, int16_t yBot);
     public:
        BackgroundGrid(int16_t _yTop, int16_t _yBot,
                        int16_t _density, int16_t factor) :
                        BackgroundLayer(factor), yTop(_yTop),
                        yBot(_yBot), density(_density) {}
        virtual void draw(SpriteRenderer* renderer,
                          const FP32& cameraPosition);
    };

    class BackgroundSprite : public BackgroundLayer {
        int16_t yPos;
        int16_t width;
        int16_t frame;
     public:
        BackgroundSprite(int16_t y, int16_t width,
                            int16_t frame, int16_t factor) :
                            BackgroundLayer(factor), yPos(y),
                            width(width), frame(frame) {
            }
        virtual void draw(SpriteRenderer* renderer,
                          const FP32& cameraPosition);
    };

    class BackgroundChopper : public BackgroundLayer {
        bool waiting;
        int16_t yPos;
        int16_t timer;
        FP32 xPos;
        FP32 xSpeed;    // in px/sec
        SpriteAnimator chopperAnim;
     public:
        BackgroundChopper();
        virtual void draw(SpriteRenderer* renderer,
                          const FP32& cameraPosition);
        virtual void update(int16_t dt);
     private:
        void restart();
        void wait(bool isWaiting);
    };
    FP32 cameraPosition;
    Car mainCar;

    BackgroundLayer* bgLayers[NB_BG_LAYERS];
    void drawLevelBackground(SpriteRenderer* renderer);
    void drawHUD(SpriteRenderer* renderer);
    void drawMainCarHUD(SpriteRenderer* renderer, int16_t x, int16_t y);
};

#endif  // LEVEL_H_

