// Copyright 2018 Catalin G. Manciu

#include "defs.h"

const FP32 Defs::FPHalfScrW(Defs::ScreenW / 2);
const FP32 Defs::MinRPM(1100);
const FP32 Defs::MaxRPM(8000);
const FP32 Defs::RaceLength(3500);

void Utils::fastGetDigits(uint16_t value, char* str, uint16_t nbDigits) {
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
