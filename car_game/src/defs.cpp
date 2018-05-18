// Copyright 2018 Catalin G. Manciu

#include "defs.h"

const FP32 Defs::FP_SCR_W(SCREEN_W);
const FP32 Defs::FP_HALF_SCR_W(SCREEN_W / 2);
const FP32 Defs::FP_SCR_H(SCREEN_H);
const FP32 Defs::FP_HALF_SCR_H(SCREEN_H / 2);
const FP32 Defs::MIN_RPM(1100);
const FP32 Defs::MAX_RPM(8000);

void fastGetDigits(uint16_t value, char* str, uint16_t nbDigits) {
    int16_t maxVal = 1;
    for (uint16_t p = 0; p < nbDigits; p ++) {
        maxVal *= 10;
    }
    maxVal--;
    if (value > maxVal) {
        value = maxVal;
    }
    while (nbDigits--) {
        *str-- = '0' + (value % 10);
        value /= 10;
    }
}
