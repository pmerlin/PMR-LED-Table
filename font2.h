#ifndef Wendy3x5_H
#define Wendy3x5_H

#define Wendy3x5_WIDTH 3
#define Wendy3x5_HEIGHT 5

/*
 * added to allow fontname to match header file name. 
 * as well as keep the old name for backward compability
 */

#define WendyFont3x5 Wendy3x5

//static uint8_t Wendy3x5[] PROGMEM = {
static uint8_t Wendy3x5[] = {
    
    // font data
        0x00, 0x00, 0x00,  // (space)
        0x17, 0x00, 0x00,  // !
        0x03, 0x00, 0x03,  // "
        0x0A, 0x1F, 0x0A,  // #
        0x16, 0x13, 0x1A,  // 0x
        0x09, 0x04, 0x0A,  // %
        0x0A, 0x15, 0x1A,  // &
        0x03, 0x00, 0x00,  // '
        0x00, 0x0E, 0x11,  // (
        0x11, 0x0E, 0x00,  // )
        0x06, 0x06, 0x00,  // *
        0x04, 0x0E, 0x04,  // +
        0x0C, 0x1C, 0x00,  // ,
        0x04, 0x04, 0x04,  // -
        0x10, 0x00, 0x00,  // .
        0x18, 0x04, 0x03,  // /
        0x1F, 0x11, 0x1F,  // 0
        0x12, 0x1F, 0x10,  // 1  0x02, 0x1F, 0x00,
        0x1D, 0x15, 0x17,  // 2
        0x15, 0x15, 0x1F,  // 3
        0x0F, 0x08, 0x1E,  // 4
        0x17, 0x15, 0x1D,  // 5
        0x1F, 0x15, 0x1D,  // 6
        0x01, 0x01, 0x1F,  // 7
        0x1F, 0x15, 0x1F,  // 8
        0x17, 0x15, 0x1F,  // 9
        0x00, 0x0A, 0x00,  // :
        0x00, 0x1A, 0x00,  // ;
        0x04, 0x0A, 0x11,  // <
        0x0A, 0x0A, 0x0A,  // =
        0x11, 0x0A, 0x04,  // >
        0x00, 0x15, 0x07,  // ?
        0x1F, 0x15, 0x17,  // @
        0x1F, 0x05, 0x1F,  // A
        0x1F, 0x15, 0x1B,  // B
        0x1F, 0x11, 0x11,  // C
        0x1F, 0x11, 0x0E,  // D
        0x1F, 0x15, 0x15,  // E
        0x1F, 0x05, 0x01,  // F
        0x1F, 0x11, 0x1D,  // G
        0x1F, 0x04, 0x1F,  // H
        0x11, 0x1F, 0x11,  // I
        0x08, 0x10, 0x0F,  // J
        0x1F, 0x04, 0x1B,  // K
        0x1F, 0x10, 0x10,  // L
        0x1F, 0x01, 0x1F,  // M 0x1F, 0x06, 0x1F,
        0x1F, 0x06, 0x1F,  // N 0x1C, 0x04, 0x1C,
        0x1F, 0x11, 0x1F,  // O
        0x1F, 0x05, 0x07,  // P
        0x0E, 0x19, 0x1E,  // Q
        0x1F, 0x05, 0x1B,  // R
        0x17, 0x15, 0x1D,  // S
        0x01, 0x1F, 0x01,  // T
        0x1F, 0x10, 0x1F,  // U
        0x0F, 0x10, 0x0F,  // V
        0x1F, 0x0C, 0x1F,  // W
        0x1B, 0x04, 0x1B,  // X
        0x17, 0x14, 0x1F,  // Y
        0x19, 0x15, 0x13,  // Z
        0x00, 0x1F, 0x11,  // [
        0x03, 0x04, 0x18,  // BackSlash
        0x11, 0x1F, 0x00,  // ]
        0x06, 0x01, 0x06,  // ^
        0x10, 0x10, 0x10,  // _
        0x01, 0x01, 0x02,  // `
        0x18, 0x14, 0x1C,  // a
        0x1F, 0x14, 0x1C,  // b
        0x1C, 0x14, 0x14,  // c
        0x1C, 0x14, 0x1F,  // d
        0x0C, 0x1A, 0x14,  // e
        0x04, 0x1E, 0x05,  // f
        0x17, 0x15, 0x1E,  // g
        0x1F, 0x04, 0x1C,  // h
        0x00, 0x1D, 0x00,  // i
        0x08, 0x10, 0x0D,  // j
        0x1F, 0x0C, 0x1A,  // k
        0x00, 0x1F, 0x00,  // l
        0x1C, 0x04, 0x1C,  // m 0x18, 0x0C, 0x18
        0x18, 0x04, 0x18,  // n
        0x1C, 0x14, 0x1C,  // o 0x1E, 0x12, 0x1E
        0x1E, 0x05, 0x07,  // p 0x1F, 0x05, 0x07,
        0x07, 0x05, 0x1F,  // q 0x07, 0x05, 0x1F,
        0x1E, 0x04, 0x04,  // r
        0x12, 0x15, 0x09,  // s
        0x00, 0x1F, 0x02,  // t 0x02, 0x1F, 0x02,
        0x1C, 0x10, 0x1C,  // u
        0x0C, 0x10, 0x0C,  // v
        0x0C, 0x18, 0x0C,  // w
        0x14, 0x08, 0x14,  // x
        0x16, 0x18, 0x06,  // y
        0x04, 0x1C, 0x10,  // z
        0x04, 0x0E, 0x11,  // {
        0x00, 0x1F, 0x00,  // |
        0x11, 0x0E, 0x04,  // }
        0x02, 0x04, 0x02,  // ~
        0x1F, 0x1F, 0x1F   // 
};

#endif
