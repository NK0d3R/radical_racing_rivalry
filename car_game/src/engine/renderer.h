// Copyright 2018 Catalin G. Manciu

#ifndef RENDERER_H_
#define RENDERER_H_

#include "../stdinc.h"
#include "../defs.h"
#include "ardusprite.h"

#define BUFFER_CAPACITY     (128)

class SpriteRenderer {
 private:
    Rect clip;
    uint8_t* frameBuffer;
    uint8_t* lineBuffer;
    uint16_t frameStride;

 public:
    SpriteRenderer() {
        lineBuffer = new uint8_t[BUFFER_CAPACITY];
    }

    ~SpriteRenderer() {
        delete[](lineBuffer);
    }

    void initialize(uint8_t* fb, uint16_t fs);
    void setClip(int16_t x, int16_t y, int16_t w, int16_t h);
    inline Rect& getClip() { return clip; }
    void drawLine(int16_t xStart, int16_t yStart,
                  int16_t xEnd, int16_t yEnd);
    void putPixel(int16_t x, int16_t y);
    void drawSpriteData(uint8_t* spriteData, int16_t targetX,
                        int16_t targetY, uint8_t width,
                        uint8_t height, uint8_t flags);
    void drawSpriteData1Bit(uint8_t* spriteData, uint8_t srcX,
                            uint8_t srcY, uint8_t targetX,
                            uint8_t targetY, int8_t width,
                            int8_t height, uint8_t initialWidth,
                            uint8_t initialHeight, uint8_t flags);
    inline uint8_t bitReverse(uint8_t byte);
    inline uint8_t leftMask(uint8_t nbBits);
};

#endif  // RENDERER_H_
