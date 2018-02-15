#include "ardusprite.h"
#include "renderer.h"

void Sprite::Create(const uint8_t* data) {
    int total_aframes = 0;
    int total_felems = 0;

    flags = pgm_read_byte(data++);
    nb_elems = pgm_read_byte(data++);
    nb_anims = pgm_read_byte(data++);

    elements = (SpriteElement*) data;
    data += (nb_elems) * sizeof(SpriteElement);
    anims = (SpriteAnim*) data;

    data += (nb_anims) * sizeof(SpriteAnim);
    anim_frames = (SpriteAnimFrame*) data;
    for (int anim = 0; anim < nb_anims; ++anim) {
        SpriteAnim current;
        memcpy_P(&current, &anims[anim], sizeof(SpriteAnim));
        total_aframes += current.nb_frames;
    }

    data += total_aframes * sizeof(SpriteAnimFrame);
    frame_elements = (SpriteFrameElement*) data;
    for (int anim = 0; anim < nb_anims; ++anim) {
        SpriteAnim current_anim;
        memcpy_P(&current_anim, &anims[anim], sizeof(SpriteAnim));
        for (uint8_t frame = 0; frame < current_anim.nb_frames; ++frame) {
            SpriteAnimFrame current_frame;
            memcpy_P(&current_frame,
                     &anim_frames[current_anim.frames_start + frame],
                     sizeof(SpriteAnimFrame));
            total_felems += current_frame.nb_elems;
        }
    }

    data += total_felems * sizeof(SpriteFrameElement);
    image_data = data;
}

void Sprite::DrawElement(SpriteRenderer* renderer, const SpriteElement& element,
                         int16_t pos_x, int16_t pos_y, uint8_t elem_flags) {
    elem_flags |= (flags << 4);
    renderer->DrawSpriteData(image_data + element.image_offset,
                             pos_x, pos_y, element.width,
                             element.height, elem_flags);
}

void Sprite::DrawAnimationFrame(SpriteRenderer* renderer,
                                uint8_t animation, uint8_t frame,
                                int16_t pos_x, int16_t pos_y,
                                uint8_t flags) {
    SpriteAnim current_anim;
    memcpy_P(&current_anim, &anims[animation], sizeof(SpriteAnim));

    SpriteAnimFrame current_frame;
    memcpy_P(&current_frame,
             &anim_frames[current_anim.frames_start + frame],
             sizeof(SpriteAnimFrame));

    int16_t elem_pos_x;
    uint8_t elem_flags;

    for(int idx = 0; idx < current_frame.nb_elems; ++idx) {
        SpriteFrameElement current_frame_elem;
        SpriteElement current_elem;

        memcpy_P(&current_frame_elem,
                 &frame_elements[current_frame.frame_elems_start + idx],
                 sizeof(SpriteFrameElement));

        elem_pos_x = (int16_t)current_frame_elem.pos_x;
        elem_flags = current_frame_elem.flags;
        memcpy_P(&current_elem, &elements[current_frame_elem.element_idx],
                 sizeof(SpriteElement));

        if(flags & ARD_FLAGS_FLIP_X) {
          uint8_t elem_width = current_elem.width;
          elem_pos_x = -elem_pos_x - elem_width + 1;
          elem_flags = ( ((~elem_flags) & ARD_FLAGS_FLIP_X) |
                          (elem_flags & (~ARD_FLAGS_FLIP_X)) );
        }

        DrawElement(renderer, current_elem,
                    pos_x + elem_pos_x, pos_y +
                    (int16_t)current_frame_elem.pos_y,
                    elem_flags);
    }
}

int32_t Sprite::MeasureAnimationFrame(uint8_t animation, uint8_t frame) {
    SpriteAnim current_anim;
    memcpy_P(&current_anim, &anims[animation], sizeof(SpriteAnim));

    SpriteAnimFrame current_frame;
    memcpy_P(&current_frame,
             &anim_frames[current_anim.frames_start + frame],
             sizeof(SpriteAnimFrame));

    int16_t width = 0;
    int16_t height = 0;

    int16_t crt_width = 0;
    int16_t crt_height = 0;

    for(int idx = 0; idx < current_frame.nb_elems; ++idx)
    {
        SpriteFrameElement current_frame_elem;
        SpriteElement current_elem;

        memcpy_P(&current_frame_elem,
                 &frame_elements[current_frame.frame_elems_start + idx],
                 sizeof(SpriteFrameElement));
        memcpy_P(&current_elem, &elements[current_frame_elem.element_idx],
                 sizeof(SpriteElement));

        crt_width = (int16_t)current_frame_elem.pos_x +
                                (int16_t)current_elem.width;
        crt_height = (int16_t)current_frame_elem.pos_y +
                                (int16_t)current_elem.height;
        if(crt_width > width) {
            width = crt_width;
        }
        if(crt_height > height) {
            height = crt_height;
        }
    }
    return (width | (height << 16));
}

void SpriteAnimator::Init(Sprite* animated_sprite) {
    sprite = animated_sprite;
    is_playing = false;
}

void SpriteAnimator::SetAnimation(uint8_t animation, uint8_t flags, bool loop) {
    current_anim = animation;
    current_anim_frame = 0;
    current_anim_flags = flags;
    current_frame_time = 0;
    loop_anim = loop;
    is_playing = true;
}

bool SpriteAnimator::Update(uint16_t dt)
{
    if(is_playing == false) {
        return false;
    }
    SpriteAnim anim;
    SpriteAnimFrame current_frame;
    memcpy_P(&anim, &sprite->anims[current_anim], sizeof(SpriteAnim));
    memcpy_P(&current_frame,
             &sprite->anim_frames[anim.frames_start +
                                  current_anim_frame],
             sizeof(SpriteAnimFrame));

    uint16_t current_duration = (uint16_t)current_frame.duration * 10;
    current_frame_time += dt;

    while(current_frame_time > current_duration) {
        current_frame_time -= current_duration;
        if(current_anim_frame < anim.nb_frames - 1) {
            current_anim_frame ++;
        }
        else if(loop_anim) {
            current_anim_frame = 0;
        }
        else {
            is_playing = false;
            return false;
        }
        memcpy_P(&current_frame,
                 &sprite->anim_frames[anim.frames_start +
                                      current_anim_frame],
                 sizeof(SpriteAnimFrame));
        current_duration = (uint16_t)current_frame.duration * 10;
    }
    return true;
}

void SpriteAnimator::Draw(SpriteRenderer* renderer,
                          int16_t pos_x, int16_t pos_y)
{
    sprite->DrawAnimationFrame(renderer, current_anim,
                               current_anim_frame, pos_x, pos_y,
                               current_anim_flags);
}

void Font::Create(const uint8_t* data, const uint8_t* map_data, uint8_t map_len,
                  uint8_t space_w, uint8_t height, uint8_t def_frame) {
    mapping = map_data;
    mapping_len = map_len;
    space_width = space_w;
    font_height = height;
    default_frame = def_frame;
    Sprite::Create(data);
}

#define MAX_STR_SIZE_BUFF   (64)
uint8_t str_size_buff[MAX_STR_SIZE_BUFF];

uint16_t Font::GetStringWidth(const char* string, int8_t char_spacing) {
    uint16_t width = 0;
    uint8_t  idx = 0;
    uint8_t  crt_frame;
    uint16_t crt_frame_w;
    static int test = 1;

    while(string[idx] && idx < MAX_STR_SIZE_BUFF) {
        if(string[idx] < mapping_len) {
            crt_frame = pgm_read_byte(&mapping[string[idx]]);
            if (test == 1) {
                for(int i = 0; i < mapping_len; ++ i){
                    Serial.println(pgm_read_byte(mapping +i));
                }
                test = 0;
            }
            if(crt_frame != default_frame) {
                crt_frame_w = GET_W_FROM_SIZE(
                                MeasureAnimationFrame(0, crt_frame));
            } else {
                crt_frame_w = space_width;
            }
        } else {
            crt_frame_w = space_width;
        }
        str_size_buff[idx] = crt_frame_w;
        width += crt_frame_w;
        ++idx;

        if(string[idx]) {
            width += char_spacing;
        }
    }
    return width;
}

void Font::DrawString(SpriteRenderer* renderer, const char* string,
                      int16_t pos_x, int16_t pos_y,
                      uint8_t anchor, int8_t char_spacing) {
    uint8_t crt_frame;
    int16_t width = GetStringWidth(string, char_spacing);
    uint8_t idx = 0;

    if(anchor & (ANCHOR_HCENTER | ANCHOR_RIGHT)) {
        if(anchor & ANCHOR_HCENTER) {
            pos_x -= (width >> 1);
        } else {
            pos_x -= width;
        }
    }
    if(anchor & ANCHOR_VCENTER) {
        pos_y += (font_height >> 1);
    } else if(anchor & ANCHOR_TOP) {
        pos_y += font_height;
    }
    while(string[idx] && idx < MAX_STR_SIZE_BUFF) {
        if(string[idx] < mapping_len) {
            crt_frame = pgm_read_byte(&mapping[string[idx]]);
            if(crt_frame != default_frame) {
                DrawAnimationFrame(renderer, 0, crt_frame, pos_x, pos_y, 0);
            }
        }
        pos_x += str_size_buff[idx];
        ++idx;
    }
}

