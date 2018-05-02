// Copyright 2018 Catalin G. Manciu

#ifndef DEFS_H_
#define DEFS_H_

#include "engine/primitives.h"
#include "engine/fixedpoint.h"

typedef FPValue<int32_t, int64_t, 8> FP32;
typedef VectorT<FP32> Vector;
typedef LineT<FP32> Line;

#define SCREEN_W    128
#define SCREEN_H    64

#define SPRITE_ENV      (0)
#define SPRITE_CAR      (1)
#define NB_SPRITES      (2)

#define CAR_BODY_ANIM       (0)
#define CAR_WHEELS_ANIM     (1)
#define CAR_REFLECTION_ANIM (2)
#define CAR_EXPLOSION_ANIM  (3)
#define CAR_RPM_HUD         (4)
#define CAR_SPEED_FONT      (5)
#define CAR_GEARS_AUTO      (6)
#define CAR_GEARS_MANUAL    (7)

#define HUD_FRAME_RPM       (0)
#define HUD_FRAME_RPM_BAR   (1)
#define HUD_FRAME_KPH       (2)
#define HUD_FRAME_MPH       (3)
#define HUD_FRAME_WARNING   (4)

#define HUD_GEARS_M_UP      (6)
#define HUD_GEARS_M_DOWN    (7)
#define HUD_GEARS_M_BG      (8)

#define HUD_GEARS_A_BG      (0)
#define HUD_GEARS_A_STICK   (1)

#define BG_CHOPPER_MARGIN_OFFSET    (15)
#define BG_CHOPPER_WAIT_TIME_MIN    (3000)
#define BG_CHOPPER_WAIT_TIME_MAX    (5000)
#define BG_CHOPPER_DECISION_TIME    (1500)

#define MAX_RPM_BAR_LENGTH  (32)
#define CAR_SPEED_FONT_W    (5)

#define FONT_MAIN       (0)
#define NB_FONTS        (1)

#define BACKGROUND_ANIM             (0)
#define BACKGROUND_SUN_FRM          (0)
#define BACKGROUND_LAYER_1          (1)
#define BACKGROUND_LAYER_2          (2)

#define BACKGROUND_CHOPPER_ANIM     (1)

#define PRECISION_SHIFT             (8)
#define STATE_INVALID               (0xFF)
#define ABS(x)                      (x < 0 ? -x : x)
#define SGN(x)                      (x < 0 ? -1 : 1)
#define SGNZ(x)                     (x == 0 ? 0 : SGN(x))
#define CLAMP_LOWER(x, limit)       (x = (x < limit) ? limit : x)
#define CLAMP_UPPER(x, limit)       (x = (x > limit) ? limit : x)
#define MPS_TO_KPH(x)               ((x * 36) / 10)
#define MIN(x, y)                   ((x) < (y) ? (x) : (y))
#define PIXELS_TO_M(x)              ((x) / 10)
#define M_TO_PIXELS(x)              ((x) * 10)

#define MAIN_FONT_HEIGHT            (7)
#define MAIN_FONT_SPACE_W           (4)

#define LEVEL_ACTION_AREA_TOP       (48)
#define LEVEL_ACTION_AREA_BOT       (63)

#define MAX_GEAR                    (5)

struct Defs {
    static const FP32 FP_SCR_W;
    static const FP32 FP_HALF_SCR_W;
    static const FP32 FP_SCR_H;
    static const FP32 FP_HALF_SCR_H;
    static const FP32 MIN_RPM;
    static const FP32 MAX_RPM;
};
#endif  // DEFS_H__

