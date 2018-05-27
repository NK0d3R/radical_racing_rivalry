// Copyright 2018 Catalin G. Manciu

#include "defs.h"

const FP32 Defs::FPHalfScrW(Defs::ScreenW / 2);
const FP32 Defs::MinRPM(1100);
const FP32 Defs::MaxRPM(8000);
const FP32 Defs::RaceLength(1500);

void Utils::fastGetDigits(uint16_t value, char* dest, uint16_t nbDigits) {
    int16_t maxVal = 1;
    for (uint16_t p = 0; p < nbDigits; p ++) {
        maxVal *= 10;
    }
    maxVal--;
    if (value > maxVal) {
        value = maxVal;
    }
    while (nbDigits--) {
        *dest-- = '0' + (value % 10);
        value /= 10;
    }
}

void Utils::formatTime(int32_t time, char* dest) {
    uint16_t sec = time / 1000;
    uint16_t msec = time - (sec * 1000);
    uint16_t min = sec / 60;
    sec -= min * 60;
    dest[9] = 0;
    Utils::fastGetDigits(msec, &dest[8], 3);
    Utils::fastGetDigits(sec, &dest[4], 2);
    Utils::fastGetDigits(min, &dest[1], 2);
    dest[5] = '.';
    dest[2] = ':';
}
