#include "ardusprite.h"
#include "renderer.h"

bool CreateSprite(Sprite* sprite, uint8_t* data) {
    sprite->data = data;
    sprite->flags = pgm_read_byte(data++);
    sprite->nb_elems = pgm_read_byte(data++);
    sprite->nb_anims = pgm_read_byte(data++);

    sprite->elements = (SpriteElement*) data;
    data += (sprite->nb_elems) * sizeof(SpriteElement);
    sprite->anims = (SpriteAnim*) data;
    data += (sprite->nb_anims) * sizeof(SpriteAnim);

    sprite->anim_frames = (SpriteAnimFrame*) data;

    int totalAnimFrames = 0;

    for (int anim = 0; anim < sprite->nb_anims; ++anim) {
        SpriteAnim current;
        memcpy_P(&current, &sprite->anims[anim], sizeof(SpriteAnim));
        totalAnimFrames += current.nb_frames;
    }

    data += totalAnimFrames * sizeof(SpriteAnimFrame);

    sprite->frame_elements = (SpriteFrameElement*) data;

    int totalFrameElems = 0;

    for (int anim = 0; anim < sprite->nb_anims; ++anim) {
        SpriteAnim current_anim;
        memcpy_P(&current_anim, &sprite->anims[anim], sizeof(SpriteAnim));
        for (uint8_t frame = 0; frame < current_anim.nb_frames; ++frame) {
            SpriteAnimFrame current_frame;
            memcpy_P(&current_frame,
                     &sprite->anim_frames[current_anim.frames_start + frame],
                     sizeof(SpriteAnimFrame));
            totalFrameElems += current_frame.nb_elems;
        }
    }

    data += totalFrameElems * sizeof(SpriteFrameElement);
    sprite->image_data = data;
    return true;
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
    SpriteAnim currentAnim;
    memcpy_P(&currentAnim, &anims[animation], sizeof(SpriteAnim));

    SpriteAnimFrame currentFrame;
    memcpy_P(&currentFrame,
             &anim_frames[currentAnim.frames_start + frame],
             sizeof(SpriteAnimFrame));

    int16_t elem_pos_x;
    uint8_t elem_flags;

    for(int idx = 0; idx < currentFrame.nb_elems; ++idx) {
        SpriteFrameElement currentFrameElem;
        SpriteElement currentElem;

        memcpy_P(&currentFrameElem,
                 &frame_elements[currentFrame.frame_elems_start + idx],
                 sizeof(SpriteFrameElement));

        elem_pos_x = (int16_t)currentFrameElem.pos_x;
        elem_flags = currentFrameElem.flags;
        memcpy_P(&currentElem, &elements[currentFrameElem.element_idx],
                 sizeof(SpriteElement));

        if(flags & ARD_FLAGS_FLIP_X) {
          uint8_t elem_width = currentElem.width;
          elem_pos_x = -elem_pos_x - elem_width + 1;
          elem_flags = ( ((~elem_flags) & ARD_FLAGS_FLIP_X) |
                          (elem_flags & (~ARD_FLAGS_FLIP_X)) );
        }

        DrawElement(renderer, currentElem,
                    pos_x + elem_pos_x, pos_y +
                    (int16_t) currentFrameElem.pos_y,
                    elem_flags);
    }
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

    SpriteAnim currentAnim;
    memcpy_P(&currentAnim, &sprite->anims[current_anim], sizeof(SpriteAnim));
    SpriteAnimFrame currentFrame;
    memcpy_P(&currentFrame,
             &sprite->anim_frames[currentAnim.frames_start +
                                  current_anim_frame],
             sizeof(SpriteAnimFrame));

    uint16_t currentDuration = (uint16_t)currentFrame.duration * 10;

    current_frame_time += dt;

    while(current_frame_time > currentDuration) {
        current_frame_time -= currentDuration;

        if(current_anim_frame < currentAnim.nb_frames - 1) {
            current_anim_frame ++;
        }
        else if(loop_anim) {
            current_anim_frame = 0;
        }
        else {
            is_playing = false;
            return false;
        }
        memcpy_P(&currentFrame,
                 &sprite->anim_frames[currentAnim.frames_start +
                                      current_anim_frame],
                 sizeof(SpriteAnimFrame));
        currentDuration = (uint16_t)currentFrame.duration * 10;
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

