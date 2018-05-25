// Copyright 2018 Catalin G. Manciu

#ifndef DEFS_H_
#define DEFS_H_

#include "engine/primitives.h"
#include "engine/fixedpoint.h"

typedef FPValue<int32_t, int64_t, 8> FP32;
typedef VectorT<FP32> Vector;
typedef LineT<FP32> Line;

int32_t getFrameCounter();

enum AppState {
    Invalid,
    MainMenu,
    Ingame,
    AfterGameMenu
};

void setAppState(AppState newState);

struct Defs {
    static constexpr int16_t ScreenW = 128;
    static constexpr int16_t ScreenH = 64;

    // Sprites
    enum : uint8_t {
        SpriteEnv = 0,
        SpriteCar,
        SpriteMenu,
        SpritesNb
    };

    // Font
    enum : uint8_t {
        FontMain = 0,
        FontsNb
    };

    // SpriteEnv anims
    enum : uint8_t {
        AnimBackgrounds = 0,
        AnimChopper  // Currently removed from sprite
    };

    // AnimBackgrounds frames
    enum : uint8_t {
        BackgroundSun = 0,
        BackgroundLayer1,
        BackgroundLayer2
    };

    // SpriteCar anims
    enum : uint8_t {
        AnimCarBody = 0,
        AnimCarWheels,
        AnimCarReflection,
        AnimCarExplosion,
        AnimCarRPMHud,
        AnimCarSpeedFont,
        AnimCarGearsAuto,
        AnimCarGearsManual,
        AnimCarCountdown
    };

    // AnimCarRPMHud frames
    enum : uint8_t {
        HUDFrameRPM = 0,
        HUDFrameRPMBar,
        HUDFrameKPH,
        HUDFrameMPH,
        HUDFrameWarning
    };

    // AnimCarGearsManual frames
    enum : uint8_t {
        HUDGearsManualUp = 6,
        HUDGearsManualDown,
        HUDGearsManualBg
    };

    // AnimCarGearsAuto frames
    enum : uint8_t {
        HUDGearsAutoBg = 0,
        HUDGearsAutoStick
    };

    // SpriteMenu anims
    enum : uint8_t {
        AnimMenuElements = 0,
        AnimMenuMain
    };

    // AnimMenuElements frames
    enum : uint8_t {
        MenuItemBg = 0,
        MenuArrowR = 2,
        MenuArrowL
    };

    // Menu actions
    enum : uint8_t {
        MenuActionStart = 1,
        MenuActionRestart,
        MenuActionBackToMM,
    };

    static constexpr int8_t BgChopperMarginOffset = 15;
    static constexpr int16_t BgChopperMinWaitTime = 3000;
    static constexpr int16_t BgChopperMaxWaitTime = 5000;
    static constexpr int16_t BgChopperDecisionTime = 1500;

    static constexpr int16_t RPMBarLength = 32;
    static constexpr int8_t CarSpeedFontW = 5;

    static constexpr uint8_t MainFontHeight = 7;
    static constexpr uint8_t MainFontSpaceW = 5;

    static constexpr int16_t LevelActionAreaTop = 45;
    static constexpr int16_t LevelActionAreaBottom = 63;

    static constexpr int8_t MaxGear = 5;

    static const FP32 FPHalfScrW;
    static const FP32 MinRPM;
    static const FP32 MaxRPM;
    static const FP32 RaceLength;
};

struct Utils {
    template<typename T>
    static T sgn(T x) { return x < 0 ? -1 : 1; }

    template<typename T>
    static T sgnz(T x) { return x == 0 ? 0 : Utils::sgn(x); }

    template<typename T>
    static T pixelsToMeters(T x) { return (x / 10); }

    template<typename T>
    static T metersToPixels(T x) { return (x * 10); }

    template<typename T>
    static T mpsToKph(T x) { return (x * 36) / 10; }

    template<typename T>
    static T upperClamp(T x, T limit) { return x > limit ? limit : x; }

    static void fastGetDigits(uint16_t value, char* str, uint16_t nbDigits);
};

#endif  // DEFS_H__
