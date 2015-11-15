/* Arduino MAX7219/7221 Library
 * See the README file for author and licensing information. In case it's
 * missing from your distribution, use the one here as the authoritative
 * version: https://github.com/csdexter/MAX7219/blob/master/README
 *
 * This library is for use with Maxim's MAX7219 and MAX7221 LED driver chips.
 * Austria Micro Systems' AS1100/1106/1107 is a pin-for-pin compatible and is
 * also supported, including its extra functionality in register 0xE.
 * See the example sketches to learn how to use the library in your code.
 *
 * This file contains definitions that are only used by the MAX7219 class and
 * shouldn't be needed by most users.
 */

#ifndef _MAX7219_PRIVATE_H_INCLUDED
#define _MAX7219_PRIVATE_H_INCLUDED

#define _MAX7219_7SEGMENT_SPACE 0x0F
#define _MAX7219_16SEGMENT_FONT_START ' '
#define _MAX7219_16SEGMENT_SPACE 0
#define _MAX7219_16SEGMENT_ZERO 16
#define _MAX7219_14SEGMENT_FONT_START ' '
#define _MAX7219_14SEGMENT_SPACE 0
#define _MAX7219_14SEGMENT_ZERO 16

// Font for 16-segment displays (MAX7219 doesn't have a built-in character
// generator for those). One word per character (high byte into chip 0, low byte
// into chip 1), one bit per segment, display-side DP is not connected and you
// need two cascaded MAX7219s to drive 16-segment displays. You can drive up to
// eight such displays, of course, but you need two chips as the displays are
// bus-wise twice as "wide" as their 7-segment counterparts.
// Mapping goes as follows (chip/digit/segment on MAX7219 => segment on
// display):
// C0/D0/DP=>A1, C0/D0/A=>A2, C0/D0/B=>B, C0/D0/C=>C, C0/D0/D=>D1, C0/D0/E=>D2,
// C0/D0/F=>E, C0/D0/G=>F, C1/D0/DP=>G1, C1/D0/A=>G2, C1/D0/B=>H, C1/D0/C=>I,
// C1/D0/D=>J, C1/D0/E=>K, C1/D0/F=>L, C1/D0/G=>M.
// Font begins with ASCII 0x20, also known as space.
const word MAX7219_16Seg_Font[] PROGMEM = {
    /* ' ' to '(' */
    0x0000, 0x3000, 0x2010, 0x03D2, 0xDDD2, 0x95DB, 0x8EB4, 0x0010, 0x000C,
    /* ')' to '1' */
    0x0021, 0x00FF, 0x00D2, 0x0001, 0x00C0, 0x0800, 0x0009, 0xFF09, 0x3008,
    /* '2' to ':' */
    0xEED0, 0xFD40, 0x31D0, 0xDDC0, 0x9FC0, 0xF000, 0xFFC0, 0xF5C0, 0x0012,
    /* ';' to 'C' */
    0x0011, 0x008C, 0x0CC0, 0x0061, 0xD042, 0xEF48, 0xF3C0, 0xFC52, 0xCF00,
    /* 'D' to 'L' */
    0xFC12, 0xCF80, 0xC380, 0xDF40, 0x33C0, 0xCC12, 0x3E00, 0x038C, 0x0F00,
    /* 'M' to 'U' */
    0x3328, 0x3324, 0xFF00, 0xE3C0, 0xFF04, 0xE3C4, 0xDDC0, 0xC012, 0x3F00,
    /* 'V' to '^' */
    0x0309, 0x3305, 0x002D, 0x21C2, 0xCC09, 0x4412, 0x0024, 0x8812, 0x0005,
    /* '_' to 'g' */
    0x0C00, 0x0020, 0x0AA2, 0x0B82, 0x0A80, 0x0A92, 0x0E81, 0x40D2, 0x8992,
    /* 'h' to 'p' */
    0x0382, 0x0002, 0x0A12, 0x001E, 0x0012, 0x12C2, 0x0282, 0x0A82, 0x8390,
    /* 'q' to 'y' */
    0x8192, 0x0280, 0x8982, 0x04D2, 0x0A02, 0x0201, 0x1205, 0x001B, 0x002A,
    /* 'z' to '~' */
    0x0882, 0x4492, 0x0300, 0x8852, 0xA150
};

// Font for 14-segment displays (MAX7219 doesn't have a built-in character
// generator for those). One word per character (high byte into chip 0, low byte
// into chip 1), one bit per segment or DP and you need two cascaded MAX7219s to
// drive 14-segment displays. You can drive up to eight such displays, of
// course, but you need two chips as the displays are bus-wise twice as "wide"
// as their 7-segment counterparts.
// Mapping goes as follows (chip/digit/segment on MAX7219 => segment on
// display):
// C0/D0/DP=>DP1, C0/D0/A=>A, C0/D0/B=>B, C0/D0/C=>C, C0/D0/D=>D, C0/D0/E=>E,
// C0/D0/F=>F, C0/D0/G=>G1, C1/D0/DP=>DP2, C1/D0/A=>G2, C1/D0/B=>H, C1/D0/C=>I,
// C1/D0/D=>J, C1/D0/E=>K, C1/D0/F=>L, C1/D0/G=>M.
// Typographical convention regarding the two DPs follows 7-segment built-in
// decoding: DP1 is the one that is placed where a decimal separator would
// occur; DP2 is the one that is placed where a thousands' separator would
// appear (if present).
// Font begins with ASCII 0x20, also known as space.
const word MAX7219_14Seg_Font[] PROGMEM = {
    /* ' ' to '(' */
    0x0000, 0xB000, 0x2010, 0x0752, 0x5B52, 0x1209, 0x4D34, 0x0010, 0x000C,
    /* ')' to '1' */
    0x0021, 0x017F, 0x0152, 0x0001, 0x0140, 0x0004, 0x0009, 0x7E09, 0x3008,
    /* '2' to ':' */
    0x6D40, 0x7840, 0x3340, 0x5D40, 0x5F40, 0x7000, 0x7F40, 0x7B40, 0x0012,
    /* ';' to 'C' */
    0x0011, 0x0114, 0x0940, 0x0061, 0xE042, 0x6E48, 0x7740, 0x7842, 0x4E00,
    /* 'D' to 'L' */
    0x7812, 0x4F00, 0x4700, 0x5E40, 0x3740, 0x4812, 0x3C00, 0x07C0, 0x0E00,
    /* 'M' to 'U' */
    0x3628, 0x3624, 0x7E00, 0x6740, 0x7E04, 0x6744, 0x5D40, 0x4012, 0x3E00,
    /* 'V' to '^' */
    0x0609, 0x3605, 0x002D, 0x2342, 0x4809, 0x4E00, 0x0024, 0x7800, 0x0041,
    /* '_' to 'g' */
    0x0800, 0x0020, 0x7D40, 0x1F40, 0x0D40, 0x3540, 0x6F40, 0x014C, 0x7860,
    /* 'h' to 'p' */
    0x1740, 0x0010, 0x3800, 0x001E, 0x0812, 0x1542, 0x0502, 0x1D40, 0x4708,
    /* 'q' to 'y' */
    0x7060, 0x0540, 0x0844, 0x0F00, 0x1C00, 0x0401, 0x1405, 0x002D, 0x002A,
    /* 'z' to '~' */
    0x0901, 0x4921, 0x0600, 0x484C, 0x0230
};

#endif
