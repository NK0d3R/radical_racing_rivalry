// Copyright 2018 Catalin G. Manciu

#ifndef DEFS_H_
#define DEFS_H_

#include "primitives.h"
#include "fixedpoint.h"

typedef FPValue<int32_t, 8> FP32;
typedef VectorT<FP32> Vector;
typedef LineT<FP32> Line;

#define SCREEN_W    128
#define SCREEN_H    64

#define SPRITE_ENV      (0)
#define SPRITE_CAR      (1)
#define NB_SPRITES      (2)

#define FONT_MAIN       (0)
#define NB_FONTS        (1)

#define BACKGROUND_ANIM             (0)
#define BACKGROUND_SUN_FRM          (0)
#define BACKGROUND_LAYER_1          (1)

#define PRECISION_SHIFT             (8)
#define STATE_INVALID               (0xFF)
#define ABS(x)                      (x < 0 ? -x : x)
#define SGN(x)                      (x < 0 ? -1 : 1)
#define SGNZ(x)                     (x == 0 ? 0 : SGN(x))
#define CLAMP_LOWER(x, limit)       (x = (x < limit) ? limit : x)
#define CLAMP_UPPER(x, limit)       (x = (x > limit) ? limit : x)
#define MPS_TO_KPH(x)               ((x * 36) / 10)

#define PIXELS_TO_M(x)              ((x) * 10)
#define M_TO_PIXELS(x)              ((x) / 10)

#define MAIN_FONT_HEIGHT            (7)
#define MAIN_FONT_SPACE_W           (4)

struct Defs {
    static const FP32 FP_SCR_W;
    static const FP32 FP_HALF_SCR_W;
    static const FP32 FP_SCR_H;
    static const FP32 FP_HALF_SCR_H;
};
#endif  // DEFS_H__

