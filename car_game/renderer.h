#ifndef __RENDERER_H__
#define __RENDERER_H__

#include "defs.h"
#include "primitives.h"
#include "ardusprite.h"

#define BUFFER_CAPACITY     (128)

class SpriteRenderer
{
    private:
        Rect clip;
        uint8_t* frame_buffer;
        uint8_t* line_buffer;
        uint16_t frame_stride;

    public:
        SpriteRenderer()
        {
            line_buffer = new uint8_t[BUFFER_CAPACITY];
        }

        ~SpriteRenderer()
        {
            delete[](line_buffer);
        }

        void Initialize(uint8_t* _frame_buffer, uint16_t _frame_stride);
        void SetClip(int16_t x, int16_t y, int16_t w, int16_t h);

        void DrawElement(Sprite* sprite, const SpriteElement& element,
                         int16_t pos_x, int16_t pos_y, uint8_t flags);
        void DrawAnimationFrame(Sprite* sprite, uint8_t animation,
                                uint8_t frame, int16_t pos_x,
                                int16_t pos_y, uint8_t flags);
        void DrawLine(const Line& line);
        void PutPixel(const Vector& point);
        void PutPixelClip(const Vector& point);
        void ClipLineX(Line& line);

      private:
        void DrawSpriteData(uint8_t* sprite_data, int16_t target_x,
                            int16_t target_y, uint8_t width,
                            uint8_t height, uint8_t flags);
        void DrawSpriteData1Bit(uint8_t* sprite_data, uint8_t src_x,
                                uint8_t src_y, uint8_t target_x,
                                uint8_t target_y, int8_t width,
                                int8_t height, uint8_t initial_width,
                                uint8_t initial_height, uint8_t flags);
};

#endif //__DEFS_H__
