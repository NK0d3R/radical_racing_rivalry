#include "renderer.h"
#include "defs.h"

const uint8_t left_masks[] = {
    0xff,
    0xfe,
    0xfc,
    0xf8,
    0xf0,
    0xe0,
    0xc0,
    0x80
};

const uint8_t bit_reverse[] = {
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

void SpriteRenderer::Initialize(uint8_t* _frame_buffer,
                                uint16_t _frame_stride) {
    frame_buffer = _frame_buffer;
    frame_stride = _frame_stride;
}

void SpriteRenderer::SetClip(int16_t x, int16_t y, int16_t w, int16_t h) {
    clip.Set(x, y, w, h);
}

void SpriteRenderer::DrawElement(Sprite* sprite, const SpriteElement& element, 
                                 int16_t pos_x, int16_t pos_y, uint8_t flags) {
    flags |= (sprite->flags << 4);
    DrawSpriteData(sprite->image_data + element.image_offset,
                   pos_x, pos_y, element.width, element.height, flags);
}

void SpriteRenderer::DrawAnimationFrame(Sprite* sprite, 
                                        uint8_t animation, uint8_t frame, 
                                        int16_t pos_x, int16_t pos_y,
                                        uint8_t flags) {
    SpriteAnim currentAnim;
    memcpy_P(&currentAnim, &sprite->anims[animation], sizeof(SpriteAnim));

    SpriteAnimFrame currentFrame;
    memcpy_P(&currentFrame,
             &sprite->anim_frames[currentAnim.frames_start + frame],
             sizeof(SpriteAnimFrame));

    int16_t elem_pos_x;
    uint8_t elem_flags;

    for(int idx = 0; idx < currentFrame.nb_elems; ++idx) {
        SpriteFrameElement currentFrameElem;
        SpriteElement currentElem;

        memcpy_P(&currentFrameElem,
                 &sprite->frame_elements[currentFrame.frame_elems_start + idx],
                 sizeof(SpriteFrameElement));

        elem_pos_x = (int16_t)currentFrameElem.pos_x;
        elem_flags = currentFrameElem.flags;
        memcpy_P(&currentElem, &sprite->elements[currentFrameElem.element_idx],
                 sizeof(SpriteElement));

        if(flags & ARD_FLAGS_FLIP_X) {
          uint8_t elem_width = currentElem.width;
          elem_pos_x = -elem_pos_x - elem_width + 1;
          elem_flags = ( ((~elem_flags) & ARD_FLAGS_FLIP_X) |
                          (elem_flags & (~ARD_FLAGS_FLIP_X)) );
        }

        DrawElement(sprite, currentElem,
                    pos_x + elem_pos_x, pos_y +
                    (int16_t) currentFrameElem.pos_y,
                    elem_flags);
    }
}

void SpriteRenderer::PutPixel(const Vector& point) {
    uint16_t dest_row_byte = point.x + (point.y >> 3) * frame_stride;
    uint8_t dest_bit = (point.y & 7);
    frame_buffer[dest_row_byte] |= (1 << dest_bit);
}

void SpriteRenderer::PutPixelClip(const Vector& point) {
    if(clip.IsInside(point)) {
        PutPixel(point);
    }
}

void SpriteRenderer::DrawSpriteData(uint8_t* sprite_data, int16_t target_x,
                                        int16_t target_y, uint8_t width,
                                        uint8_t height, uint8_t flags) {
    Rect data_bounds(target_x, target_y, width, height);

    data_bounds.Clip(clip);

    if(data_bounds.IsEmpty()) {
        return;
    }

    int16_t src_x = data_bounds.x - target_x;
    int16_t src_y = data_bounds.y - target_y;

    if(src_x < 0 || src_y < 0) {
        return;
    }

    DrawSpriteData1Bit(sprite_data, src_x, src_y,
                       data_bounds.x, data_bounds.y,
                       data_bounds.w, data_bounds.h,
                       width, height, flags);
}

void SpriteRenderer::DrawSpriteData1Bit(uint8_t* sprite_data, uint8_t src_x,
                                        uint8_t src_y, uint8_t target_x,
                                        uint8_t target_y, int8_t width, 
                                        int8_t height, uint8_t initial_width,
                                        uint8_t initial_height, uint8_t flags) {
    bool x_flipped = (flags & ARD_FLAGS_FLIP_X);
    bool y_flipped = (flags & ARD_FLAGS_FLIP_Y);
    
    uint16_t sprite_data_stride = (initial_width << 1);    
    uint16_t dest_row_start_byte = target_x + (target_y >> 3) * frame_stride;

    uint8_t dest_bit = (target_y & 7);

    uint8_t current_pix;
    uint8_t current_mask;

    uint16_t src_row_start_byte;
    uint8_t  src_bit;
    int16_t  src_row_incr;

    uint16_t dest_offset;

    uint8_t to_shift;
    bool shift_left;

    uint8_t pix_written;

    int8_t x_offset = 0;
    int8_t x_offset_start = 0;
    int8_t x_offset_inc = 2;
    int8_t cols_to_write;

    if(y_flipped == false)
    {
        src_row_start_byte = (src_y >> 3) * sprite_data_stride;
        src_row_incr = sprite_data_stride;
        src_bit = (src_y & 7);
    }
    else 
    {
        src_row_start_byte = (((initial_height - src_y - 1) >> 3) *
                              sprite_data_stride);
        src_row_incr = -sprite_data_stride;
        
        src_y = initial_height - src_y - 1;
        src_bit = 7 - (src_y & 7);
    }

    if(x_flipped == true)
    {
        x_offset_start = ((width - 1) << 1);
        x_offset_inc = -2;

        if(src_x == 0)
        {
            src_row_start_byte += ((initial_width - width) << 1);
        }
    }
    else
    {
        src_row_start_byte += (src_x << 1);
    }

    memcpy_P(line_buffer, sprite_data + src_row_start_byte, (width << 1));

    while(height > 0)
    {           
        if(src_bit == dest_bit) 
        {
            to_shift = 0;
            pix_written = (8 - src_bit);
        }
        else if(src_bit < dest_bit) 
        {
            to_shift = dest_bit - src_bit;
            shift_left = true;
            pix_written = (8 - dest_bit);
        } 
        else 
        {
            to_shift = src_bit - dest_bit;
            shift_left = false;
            pix_written = (8 - src_bit);
        }

        dest_offset = dest_row_start_byte;

        for(x_offset = x_offset_start, cols_to_write = width;
            cols_to_write > 0; -- cols_to_write, x_offset += x_offset_inc) 
        {
            current_pix = line_buffer[x_offset];
            current_mask = line_buffer[x_offset + 1];

            if(y_flipped != 0) 
            {
                current_pix = bit_reverse[current_pix];
                current_mask = bit_reverse[current_mask];  
            }
            
            current_mask &= left_masks[src_bit];
            
            if(to_shift != 0)
            {
                if(shift_left)
                {
                    current_mask <<= to_shift;
                    current_pix <<= to_shift;
                }
                else
                {
                    current_mask >>= to_shift;
                    current_pix >>= to_shift;
                }
            }

            frame_buffer[dest_offset] = ((current_pix & current_mask) |
                                         (frame_buffer[dest_offset] &
                                          ~current_mask));
            dest_offset ++;
        }

        height -= pix_written;
        src_bit += pix_written;
        dest_bit += pix_written;

        if(src_bit > 7)
        {
            src_bit = 0;
            src_row_start_byte += src_row_incr;
            memcpy_P(line_buffer, sprite_data + src_row_start_byte,
                     (width << 1));
        }

        if(dest_bit > 7)
        {
            dest_bit = 0;
            dest_row_start_byte += frame_stride;
        }   
    }
}

void SpriteRenderer::DrawLine(const Line& line)
{
    Vector  diff = line.end - line.start;
    int32_t abs_x = ABS(diff.x);
    int32_t abs_y = ABS(diff.y);
    int32_t increment_x = SGNZ(diff.x);
    int32_t increment_y = SGNZ(diff.y);

    if(abs_x > abs_y) {
        int32_t d = (abs_y << 1) - abs_x;
        int32_t y = line.start.y;           
        for(int32_t x = line.start.x; x != line.end.x; x += increment_x) {
            PutPixel({x, y});
            if(d > 0) {
                y += increment_y;
                d -= (abs_x << 1);
            }
            d += (abs_y << 1);
        }
    } else {
        int32_t d = (abs_x << 1) - abs_y;
        int32_t x = line.start.x;           
        for(int32_t y = line.start.y; y != line.end.y; y += increment_y) {
            PutPixel({x, y});
            if(d > 0) {
                x += increment_x;
                d -= (abs_y << 1);
            }
            d += (abs_x << 1);
        }
    }
}

void SpriteRenderer::ClipLineX(Line& line) {
    clip.ClipLineX(line);
}

