// Copyright 2018 Catalin G. Manciu

#ifndef FONT_H_
#define FONT_H_

#define FONT_SIZE    950
PROGMEM const uint8_t FONT_DATA[] = {
    0x2a, 0x01, 0x06, 0x07, 0x00, 0x00, 0x06, 0x07,
    0x0c, 0x00, 0x06, 0x07, 0x18, 0x00, 0x06, 0x07,
    0x24, 0x00, 0x06, 0x07, 0x30, 0x00, 0x06, 0x07,
    0x3c, 0x00, 0x06, 0x07, 0x48, 0x00, 0x06, 0x87,
    0x54, 0x00, 0x03, 0x87, 0x5a, 0x00, 0x06, 0x07,
    0x5d, 0x00, 0x07, 0x07, 0x69, 0x00, 0x06, 0x07,
    0x77, 0x00, 0x07, 0x07, 0x83, 0x00, 0x07, 0x07,
    0x91, 0x00, 0x06, 0x07, 0x9f, 0x00, 0x06, 0x07,
    0xab, 0x00, 0x06, 0x08, 0xb7, 0x00, 0x06, 0x07,
    0xc3, 0x00, 0x06, 0x07, 0xcf, 0x00, 0x07, 0x07,
    0xdb, 0x00, 0x06, 0x07, 0xe9, 0x00, 0x07, 0x07,
    0xf5, 0x00, 0x07, 0x07, 0x03, 0x01, 0x07, 0x07,
    0x11, 0x01, 0x07, 0x07, 0x1f, 0x01, 0x06, 0x07,
    0x2d, 0x01, 0x06, 0x07, 0x39, 0x01, 0x04, 0x07,
    0x45, 0x01, 0x06, 0x07, 0x4d, 0x01, 0x06, 0x07,
    0x59, 0x01, 0x06, 0x07, 0x65, 0x01, 0x06, 0x07,
    0x71, 0x01, 0x06, 0x07, 0x7d, 0x01, 0x06, 0x07,
    0x89, 0x01, 0x06, 0x07, 0x95, 0x01, 0x06, 0x07,
    0xa1, 0x01, 0x03, 0x83, 0xad, 0x01, 0x04, 0x04,
    0xb0, 0x01, 0x05, 0x83, 0xb8, 0x01, 0x05, 0x05,
    0xbd, 0x01, 0x03, 0x84, 0xc7, 0x01, 0x04, 0x05,
    0xca, 0x01, 0x2c, 0x00, 0x03, 0x01, 0x00, 0x03,
    0x01, 0x01, 0x03, 0x01, 0x02, 0x03, 0x01, 0x03,
    0x03, 0x01, 0x04, 0x03, 0x01, 0x05, 0x03, 0x01,
    0x06, 0x03, 0x01, 0x07, 0x03, 0x01, 0x08, 0x03,
    0x01, 0x09, 0x03, 0x01, 0x0a, 0x03, 0x01, 0x0b,
    0x03, 0x01, 0x0c, 0x03, 0x01, 0x0d, 0x03, 0x01,
    0x0e, 0x03, 0x01, 0x0f, 0x03, 0x01, 0x10, 0x03,
    0x01, 0x11, 0x03, 0x01, 0x12, 0x03, 0x01, 0x13,
    0x03, 0x01, 0x14, 0x03, 0x01, 0x15, 0x03, 0x01,
    0x16, 0x03, 0x01, 0x17, 0x03, 0x01, 0x18, 0x03,
    0x01, 0x19, 0x03, 0x01, 0x1a, 0x03, 0x01, 0x1b,
    0x03, 0x01, 0x1c, 0x03, 0x01, 0x1d, 0x03, 0x01,
    0x1e, 0x03, 0x01, 0x1f, 0x03, 0x01, 0x20, 0x03,
    0x01, 0x21, 0x03, 0x01, 0x22, 0x03, 0x01, 0x23,
    0x03, 0x01, 0x24, 0x03, 0x01, 0x25, 0x03, 0x02,
    0x26, 0x03, 0x01, 0x28, 0x03, 0x01, 0x29, 0x03,
    0x02, 0x2a, 0x03, 0x01, 0x2c, 0x03, 0x00, 0x2d,
    0x00, 0x00, 0x00, 0xf9, 0x01, 0x00, 0x00, 0xf9,
    0x02, 0x00, 0x00, 0xf9, 0x03, 0x00, 0x00, 0xf9,
    0x04, 0x00, 0x00, 0xf9, 0x05, 0x00, 0x00, 0xf9,
    0x06, 0x00, 0x00, 0xf9, 0x07, 0x00, 0x00, 0xf9,
    0x08, 0x00, 0x00, 0xf9, 0x09, 0x00, 0x00, 0xf9,
    0x0a, 0x00, 0x00, 0xf9, 0x0b, 0x00, 0x00, 0xf9,
    0x0c, 0x00, 0x00, 0xf9, 0x0d, 0x00, 0x00, 0xf9,
    0x0e, 0x00, 0x00, 0xf9, 0x0f, 0x00, 0x00, 0xf9,
    0x10, 0x00, 0x00, 0xfa, 0x11, 0x00, 0x00, 0xf9,
    0x12, 0x00, 0x00, 0xf9, 0x13, 0x00, 0x00, 0xf9,
    0x14, 0x00, 0x00, 0xf9, 0x15, 0x00, 0x00, 0xf9,
    0x16, 0x00, 0x00, 0xf9, 0x17, 0x00, 0x00, 0xf9,
    0x18, 0x00, 0x00, 0xf9, 0x19, 0x00, 0x00, 0xf9,
    0x1a, 0x00, 0x00, 0xf9, 0x1b, 0x00, 0x00, 0xf9,
    0x1c, 0x00, 0x00, 0xf9, 0x1d, 0x00, 0x00, 0xf9,
    0x1e, 0x00, 0x00, 0xf9, 0x1f, 0x00, 0x00, 0xf9,
    0x20, 0x00, 0x00, 0xf9, 0x21, 0x00, 0x00, 0xf9,
    0x22, 0x00, 0x00, 0xf9, 0x23, 0x00, 0x00, 0xf9,
    0x24, 0x00, 0x00, 0xfd, 0x25, 0x00, 0xff, 0xfe,
    0x24, 0x00, 0x00, 0xfd, 0x24, 0x00, 0x00, 0xfa,
    0x27, 0x00, 0x00, 0xfa, 0x26, 0x00, 0x00, 0xfb,
    0x28, 0x00, 0x00, 0xf9, 0x24, 0x00, 0x00, 0xfd,
    0x29, 0x00, 0x00, 0xfa, 0x01, 0x7e, 0x3c, 0x7f,
    0x0a, 0x7f, 0x0a, 0x7f, 0x3c, 0x7f, 0x01, 0x7e,
    0x00, 0x7f, 0x3e, 0x7f, 0x2a, 0x7f, 0x2a, 0x7f,
    0x14, 0x7f, 0x49, 0x36, 0x41, 0x3e, 0x1c, 0x7f,
    0x22, 0x7f, 0x2a, 0x77, 0x2a, 0x77, 0x08, 0x77,
    0x00, 0x7f, 0x3e, 0x7f, 0x22, 0x7f, 0x22, 0x7f,
    0x1c, 0x7f, 0x41, 0x3e, 0x00, 0x7f, 0x3e, 0x7f,
    0x2a, 0x7f, 0x2a, 0x7f, 0x22, 0x7f, 0x08, 0x77,
    0x00, 0x7f, 0x3e, 0x7f, 0x0a, 0x7f, 0x6a, 0x1f,
    0x62, 0x1f, 0x78, 0x07, 0x41, 0x3e, 0x1c, 0x7f,
    0x22, 0x7f, 0x2a, 0x7f, 0x3a, 0x7f, 0x40, 0x3f,
    0x00, 0x3e, 0x08, 0x08, 0x3e, 0x00, 0x00, 0x3e,
    0x00, 0x4f, 0x30, 0x17, 0x78, 0x2f, 0x70, 0x20,
    0x7f, 0x1e, 0x7f, 0x40, 0x3f, 0x00, 0x7f, 0x3e,
    0x7f, 0x08, 0x7f, 0x14, 0x7f, 0x22, 0x7f, 0x08,
    0x77, 0x7f, 0x00, 0x00, 0x7f, 0x3e, 0x7f, 0x20,
    0x7f, 0x2f, 0x70, 0x2f, 0x70, 0x0f, 0x70, 0x00,
    0x7f, 0x3e, 0x7f, 0x04, 0x7f, 0x69, 0x1e, 0x04,
    0x7f, 0x3e, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x3e,
    0x7f, 0x04, 0x7f, 0x49, 0x3e, 0x10, 0x7f, 0x3e,
    0x7f, 0x00, 0x7f, 0x41, 0x3e, 0x1c, 0x7f, 0x22,
    0x7f, 0x22, 0x7f, 0x1c, 0x7f, 0x41, 0x3e, 0x00,
    0x7f, 0x3e, 0x7f, 0x0a, 0x7f, 0x6a, 0x1f, 0x6c,
    0x1f, 0x71, 0x0e, 0xc1, 0x3e, 0x9c, 0x7f, 0x22,
    0xff, 0x62, 0xff, 0x1c, 0xff, 0xc1, 0x3e, 0x00,
    0x7f, 0x3e, 0x7f, 0x0a, 0x7f, 0x1a, 0x7f, 0x2c,
    0x7f, 0x01, 0x7e, 0x01, 0x7e, 0x2c, 0x7f, 0x2a,
    0x7f, 0x2a, 0x7f, 0x1a, 0x7f, 0x40, 0x3f, 0x78,
    0x07, 0x7a, 0x07, 0x02, 0x7f, 0x3e, 0x7f, 0x02,
    0x7f, 0x7a, 0x07, 0x78, 0x07, 0x40, 0x3f, 0x1e,
    0x7f, 0x20, 0x7f, 0x20, 0x7f, 0x1e, 0x7f, 0x40,
    0x3f, 0x60, 0x1f, 0x4e, 0x3f, 0x10, 0x7f, 0x27,
    0x78, 0x10, 0x7f, 0x4e, 0x3f, 0x60, 0x1f, 0x40,
    0x3f, 0x1e, 0x7f, 0x20, 0x7f, 0x3b, 0x7c, 0x20,
    0x7f, 0x1e, 0x7f, 0x40, 0x3f, 0x08, 0x77, 0x22,
    0x7f, 0x14, 0x7f, 0x69, 0x1e, 0x14, 0x7f, 0x22,
    0x7f, 0x08, 0x77, 0x78, 0x07, 0x72, 0x0f, 0x04,
    0x7f, 0x39, 0x7e, 0x04, 0x7f, 0x72, 0x0f, 0x78,
    0x07, 0x08, 0x77, 0x32, 0x7f, 0x2a, 0x7f, 0x2a,
    0x7f, 0x26, 0x7f, 0x00, 0x7f, 0x41, 0x3e, 0x1e,
    0x7f, 0x22, 0x7f, 0x22, 0x7f, 0x3c, 0x7f, 0x41,
    0x3e, 0x78, 0x07, 0x02, 0x7f, 0x3e, 0x7f, 0x00,
    0x7f, 0x08, 0x77, 0x32, 0x7f, 0x2a, 0x7f, 0x2a,
    0x7f, 0x24, 0x7f, 0x01, 0x7e, 0x08, 0x77, 0x22,
    0x7f, 0x2a, 0x7f, 0x2a, 0x7f, 0x3e, 0x7f, 0x41,
    0x3e, 0x60, 0x1f, 0x6e, 0x1f, 0x68, 0x1f, 0x08,
    0x7f, 0x3e, 0x7f, 0x00, 0x7f, 0x00, 0x7f, 0x2e,
    0x7f, 0x2a, 0x7f, 0x2a, 0x7f, 0x1a, 0x7f, 0x40,
    0x3f, 0x41, 0x3e, 0x1c, 0x7f, 0x2a, 0x7f, 0x2a,
    0x7f, 0x3a, 0x7f, 0x40, 0x3f, 0x70, 0x0f, 0x76,
    0x0f, 0x72, 0x0f, 0x02, 0x7f, 0x3e, 0x7f, 0x00,
    0x7f, 0x41, 0x3e, 0x16, 0x7f, 0x2a, 0x7f, 0x2a,
    0x7f, 0x34, 0x7f, 0x41, 0x3e, 0x01, 0x7e, 0x2e,
    0x7f, 0x2a, 0x7f, 0x2a, 0x7f, 0x3c, 0x7f, 0x41,
    0x3e, 0x00, 0x02, 0x00, 0x03, 0x0c, 0x04, 0x0f,
    0x0a, 0x07, 0x08, 0x07, 0x00, 0x02, 0x02, 0x02,
    0x00, 0x11, 0x0e, 0x04, 0x1f, 0x0e, 0x1f, 0x04,
    0x1f, 0x11, 0x0e, 0x00, 0x06, 0x00, 0x00, 0x1f,
    0x0e, 0x1f, 0x15, 0x0e, 0x1b, 0x04,
};

#endif  // FONT_H_
