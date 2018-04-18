// Copyright 2018 Catalin G. Manciu

#include "renderer.h"

const uint8_t s_leftMasks[] = {
    0xff,
    0xfe,
    0xfc,
    0xf8,
    0xf0,
    0xe0,
    0xc0,
    0x80
};

const uint8_t s_bitReverse[] = {
    0x00, 0x80, 0x40, 0xC0, 0x20, 0xA0, 0x60, 0xE0,
    0x10, 0x90, 0x50, 0xD0, 0x30, 0xB0, 0x70, 0xF0,
    0x08, 0x88, 0x48, 0xC8, 0x28, 0xA8, 0x68, 0xE8,
    0x18, 0x98, 0x58, 0xD8, 0x38, 0xB8, 0x78, 0xF8,
    0x04, 0x84, 0x44, 0xC4, 0x24, 0xA4, 0x64, 0xE4,
    0x14, 0x94, 0x54, 0xD4, 0x34, 0xB4, 0x74, 0xF4,
    0x0C, 0x8C, 0x4C, 0xCC, 0x2C, 0xAC, 0x6C, 0xEC,
    0x1C, 0x9C, 0x5C, 0xDC, 0x3C, 0xBC, 0x7C, 0xFC,
    0x02, 0x82, 0x42, 0xC2, 0x22, 0xA2, 0x62, 0xE2,
    0x12, 0x92, 0x52, 0xD2, 0x32, 0xB2, 0x72, 0xF2,
    0x0A, 0x8A, 0x4A, 0xCA, 0x2A, 0xAA, 0x6A, 0xEA,
    0x1A, 0x9A, 0x5A, 0xDA, 0x3A, 0xBA, 0x7A, 0xFA,
    0x06, 0x86, 0x46, 0xC6, 0x26, 0xA6, 0x66, 0xE6,
    0x16, 0x96, 0x56, 0xD6, 0x36, 0xB6, 0x76, 0xF6,
    0x0E, 0x8E, 0x4E, 0xCE, 0x2E, 0xAE, 0x6E, 0xEE,
    0x1E, 0x9E, 0x5E, 0xDE, 0x3E, 0xBE, 0x7E, 0xFE,
    0x01, 0x81, 0x41, 0xC1, 0x21, 0xA1, 0x61, 0xE1,
    0x11, 0x91, 0x51, 0xD1, 0x31, 0xB1, 0x71, 0xF1,
    0x09, 0x89, 0x49, 0xC9, 0x29, 0xA9, 0x69, 0xE9,
    0x19, 0x99, 0x59, 0xD9, 0x39, 0xB9, 0x79, 0xF9,
    0x05, 0x85, 0x45, 0xC5, 0x25, 0xA5, 0x65, 0xE5,
    0x15, 0x95, 0x55, 0xD5, 0x35, 0xB5, 0x75, 0xF5,
    0x0D, 0x8D, 0x4D, 0xCD, 0x2D, 0xAD, 0x6D, 0xED,
    0x1D, 0x9D, 0x5D, 0xDD, 0x3D, 0xBD, 0x7D, 0xFD,
    0x03, 0x83, 0x43, 0xC3, 0x23, 0xA3, 0x63, 0xE3,
    0x13, 0x93, 0x53, 0xD3, 0x33, 0xB3, 0x73, 0xF3,
    0x0B, 0x8B, 0x4B, 0xCB, 0x2B, 0xAB, 0x6B, 0xEB,
    0x1B, 0x9B, 0x5B, 0xDB, 0x3B, 0xBB, 0x7B, 0xFB,
    0x07, 0x87, 0x47, 0xC7, 0x27, 0xA7, 0x67, 0xE7,
    0x17, 0x97, 0x57, 0xD7, 0x37, 0xB7, 0x77, 0xF7,
    0x0F, 0x8F, 0x4F, 0xCF, 0x2F, 0xAF, 0x6F, 0xEF,
    0x1F, 0x9F, 0x5F, 0xDF, 0x3F, 0xBF, 0x7F, 0xFF
};

void SpriteRenderer::initialize(uint8_t* fb, uint16_t fs) {
    frameBuffer = fb;
    frameStride = fs;
}

void SpriteRenderer::setClip(int16_t x, int16_t y, int16_t w, int16_t h) {
    clip.set(x, y, w, h);
}

void SpriteRenderer::putPixel(int16_t x, int16_t y) {
    uint16_t destRowByte = x + (y >> 3) * frameStride;
    uint8_t destBit = (y & 7);
    frameBuffer[destRowByte] |= (1 << destBit);
}

void SpriteRenderer::drawSpriteData(uint8_t* spriteData, int16_t targetX,
                                    int16_t targetY, uint8_t width,
                                    uint8_t height, uint8_t flags) {
    Rect dataBounds(targetX, targetY, width, height);

    dataBounds.clip(clip);

    if (dataBounds.isEmpty()) {
        return;
    }

    int16_t srcX = dataBounds.x - targetX;
    int16_t srcY = dataBounds.y - targetY;

    if (srcX < 0 || srcY < 0) {
        return;
    }

    drawSpriteData1Bit(spriteData, srcX, srcY, dataBounds.x, dataBounds.y,
                       dataBounds.w, dataBounds.h, width, height, flags);
}

void SpriteRenderer::drawSpriteData1Bit(uint8_t* spriteData, uint8_t srcX,
                                        uint8_t srcY, uint8_t targetX,
                                        uint8_t targetY, int8_t width,
                                        int8_t height, uint8_t initialWidth,
                                        uint8_t initialHeight, uint8_t flags) {
    bool xFlipped = (flags & ARD_FLAGS_FLIP_X);
    bool yFlipped = (flags & ARD_FLAGS_FLIP_Y);

    uint16_t spriteDataStride = (initialWidth << 1);
    uint16_t destRowStartByte = targetX + ((targetY >> 3) * frameStride);

    uint8_t destBit = (targetY & 7);

    uint8_t currentPix;
    uint8_t currentMask;

    uint16_t srcRowStartByte;
    uint8_t  srcBit;
    int16_t  srcRowIncr;

    uint16_t destOffset;

    uint8_t toShift;
    bool shiftLeft;

    uint8_t pixWritten;

    int8_t xOffset = 0;
    int8_t xOffsetStart = 0;
    int8_t xOffsetInc = 2;
    int8_t colsToWrite;

    if (yFlipped == false) {
        srcRowStartByte = (srcY >> 3) * spriteDataStride;
        srcRowIncr = spriteDataStride;
        srcBit = (srcY & 7);
    } else {
        srcRowStartByte = (((initialHeight - srcY - 1) >> 3) *
                              spriteDataStride);
        srcRowIncr = -spriteDataStride;
        srcY = initialHeight - srcY - 1;
        srcBit = 7 - (srcY & 7);
    }

    if (xFlipped == true) {
        xOffsetStart = ((width - 1) << 1);
        xOffsetInc = -2;
        if (srcX == 0) {
            srcRowStartByte += ((initialWidth - width) << 1);
        }
    } else {
        srcRowStartByte += (srcX << 1);
    }

    memcpy_P(lineBuffer, spriteData + srcRowStartByte, (width << 1));

    while (height > 0) {
        if (srcBit == destBit) {
            toShift = 0;
            pixWritten = (8 - srcBit);
        } else if (srcBit < destBit) {
            toShift = destBit - srcBit;
            shiftLeft = true;
            pixWritten = (8 - destBit);
        } else {
            toShift = srcBit - destBit;
            shiftLeft = false;
            pixWritten = (8 - srcBit);
        }

        destOffset = destRowStartByte;

        for (xOffset = xOffsetStart, colsToWrite = width;
            colsToWrite > 0; --colsToWrite, xOffset += xOffsetInc) {
            currentPix = lineBuffer[xOffset];
            currentMask = lineBuffer[xOffset + 1];
            if (yFlipped != 0) {
                currentPix = s_bitReverse[currentPix];
                currentMask = s_bitReverse[currentMask];
            }
            currentMask &= s_leftMasks[srcBit];
            if (toShift != 0) {
                if (shiftLeft) {
                    currentMask <<= toShift;
                    currentPix <<= toShift;
                } else {
                    currentMask >>= toShift;
                    currentPix >>= toShift;
                }
            }

            frameBuffer[destOffset] = ((currentPix & currentMask) |
                                       (frameBuffer[destOffset] &
                                        ~currentMask));
            destOffset++;
        }
        height -= pixWritten;
        srcBit += pixWritten;
        destBit += pixWritten;

        if (srcBit > 7) {
            srcBit = 0;
            srcRowStartByte += srcRowIncr;
            memcpy_P(lineBuffer, spriteData + srcRowStartByte, (width << 1));
        }
        if (destBit > 7) {
            destBit = 0;
            destRowStartByte += frameStride;
        }
    }
}

void SpriteRenderer::drawLine(int16_t xStart, int16_t yStart,
                              int16_t xEnd, int16_t yEnd) {
    int32_t xDiff = xEnd - xStart;
    int32_t yDiff = yEnd - yStart;
    int32_t absX = ABS(xDiff);
    int32_t absY = ABS(yDiff);
    int32_t incrementX = SGNZ(xDiff);
    int32_t incrementY = SGNZ(yDiff);

    if (absX > absY) {
        int32_t d = (absY << 1) - absX;
        int32_t y = yStart;
        for (int32_t x = xStart;; x += incrementX) {
            putPixel(x, y);
            if (x == xEnd) {
                return;
            }
            if (d > 0) {
                y += incrementY;
                d -= (absX << 1);
            }
            d += (absY << 1);
        }
    } else {
        int32_t d = (absX << 1) - absY;
        int32_t x = xStart;
        for (int32_t y = yStart;; y += incrementY) {
            putPixel(x, y);
            if (y == yEnd) {
                return;
            }
            if (d > 0) {
                x += incrementX;
                d -= (absY << 1);
            }
            d += (absX << 1);
        }
    }
}
