#ifndef __ARDUSPRITE_H__
#define __ARDUSPRITE_H__

#include "defs.h"

#define INVALID_ANIM  (0xFF)
#define INVALID_FRAME (0xFF)

#define ANCHOR_LEFT     (1)
#define ANCHOR_HCENTER  (1<<1)
#define ANCHOR_RIGHT    (1<<2)
#define ANCHOR_TOP      (1<<3)
#define ANCHOR_VCENTER  (1<<4)
#define ANCHOR_BOTTOM   (1<<5)

#define ARD_FLAGS_FLIP_X    (1)
#define ARD_FLAGS_FLIP_Y    (2)

#define GET_W_FROM_SIZE(sz) (uint16_t)(sz & 0xFFFF)
#define GET_H_FROM_SIZE(sz) (uint16_t)((sz & 0xFFFF0000) >> 16)

class SpriteRenderer;

struct SpriteElement {
    uint8_t     width;
    uint8_t     height;
    uint16_t    image_offset;
} __attribute__((__packed__));

struct SpriteFrameElement {
    uint8_t     element_idx;
    uint8_t     flags;
    int8_t      pos_x;
    int8_t      pos_y;
} __attribute__((__packed__));

struct SpriteAnimFrame {
    uint8_t   duration;
    uint8_t   nb_elems;
    
    uint16_t  frame_elems_start;
}__attribute__((__packed__));

struct SpriteAnim {
    uint8_t   nb_frames;
    uint16_t  frames_start;
}__attribute__((__packed__));

struct Sprite {
    uint8_t   flags;
    uint8_t   nb_elems;
    uint8_t   nb_anims;
    uint8_t*  image_data;
    
    SpriteElement* elements;
    SpriteAnim* anims;
    SpriteAnimFrame* anim_frames;
    SpriteFrameElement* frame_elements;

    void DrawElement(SpriteRenderer* renderer, const SpriteElement& element,
                     int16_t pos_x, int16_t pos_y, uint8_t elem_flags);
    void DrawAnimationFrame(SpriteRenderer* renderer, uint8_t animation,
                            uint8_t frame, int16_t pos_x,
                            int16_t pos_y, uint8_t flags);
    bool Create(uint8_t* data);
} __attribute__((__packed__));

class SpriteAnimator {
    Sprite*      sprite;
    uint16_t     current_frame_time;
    
    bool         loop_anim;
    bool         is_playing;
    
    uint8_t      current_anim;
    uint8_t      current_anim_frame;
    uint8_t      current_anim_flags;

    public:
        void Init(Sprite* animated_sprite);
        void SetAnimation(uint8_t animation, uint8_t flags, bool loop);
        bool Update(uint16_t dt);
        void Draw(SpriteRenderer* renderer, int16_t pos_x, int16_t pos_y);
}__attribute__((__packed__));

#endif //__ARDUSPRITE_H__

