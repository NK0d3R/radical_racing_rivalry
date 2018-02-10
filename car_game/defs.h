#ifndef __DEFS_H__
#define __DEFS_H__

#include <Arduboy.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define SCREEN_W    128
#define SCREEN_H    64

#define SPRITE_ENV     (0)
#define NB_SPRITES     (1)

#define BACKGROUND_ANIM             (0)
#define BACKGROUND_SUN_FRM          (0)
#define BACKGROUND_LAYER_1          (1)

#define PRECISION_SHIFT             (8)
#define TO_FIXED_POINT(n)           ((int32_t)(n) << PRECISION_SHIFT)
#define FROM_FIXED_POINT(n)         ((n) >> PRECISION_SHIFT)
#define FLOAT_TO_FIXED(n)           ((int32_t)(n * (float)(TO_FIXED_POINT(1))))
#define STATE_INVALID               (0xFF)
#define ABS(x)                      (x < 0 ? -x : x)
#define SGN(x)                      (x < 0 ? -1 : 1)
#define SGNZ(x)                     (x == 0 ? 0 : SGN(x))

#endif //__DEFS_H__
