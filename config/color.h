#ifndef __COLOR_H__
#define __COLOR_H__

#ifdef LOG_HEADER_INCLUDE
#pragma message("INCLUDE: color.h")
#endif 

#define RGBA_BLACK       0,      0,      0,      255
#define RGBA_WHITE       255,    255,    255,    255
#define RGBA_RED         255,    0,      0,      255
#define RGBA_GREEN       0,      255,    0,      255
#define RGBA_BLUE        0,      0,      255,    255
#define RGBA_YELLOW      255,    255,    0,      255
#define RGBA_CYAN        0,      255,    255,    255
#define RGBA_MAGENTA     255,    0,      255,    255
#define RGBA_ORANGE      255,    165,    0,      255
#define RGBA_LIME        50,     205,    50,     255
#define RGBA_PINK        255,    192,    203,    255
#define RGBA_PURPLE      128,    0,      128,    255
#define RGBA_VIOLET      238,    130,    238,    255
#define RGBA_BROWN       165,    42,     42,     255
#define RGBA_MAROON      128,    0,      0,      255
#define RGBA_NAVY        0,      0,      128,    255
#define RGBA_TEAL        0,      128,    128,    255
#define RGBA_OLIVE       128,    128,    0,      255
#define RGBA_INDIGO      75,     0,      130,    255
#define RGBA_CRIMSON     220,    20,     60,     255
#define RGBA_SKYBLUE     135,    206,    235,    255
#define RGBA_TURQUOISE   64,     224,    208,    255
#define RGBA_CORAL       255,    127,    80,     255
#define RGBA_GOLD        255,    215,    0,      255
#define RGBA_KHAKI       240,    230,    140,    255
#define RGBA_BEIGE       245,    245,    220,    255
#define RGBA_GRAY        128,    128,    128,    255
#define RGBA_DARKGRAY    169,    169,    169,    255
#define RGBA_LIGHTGRAY   211,    211,    211,    255
#define RGBA_SILVER      192,    192,    192,    255

#define HEX32_BLACK       0x000000FF
#define HEX32_WHITE       0xFFFFFFFF
#define HEX32_RED         0xFF0000FF
#define HEX32_GREEN       0x00FF00FF
#define HEX32_BLUE        0x0000FFFF
#define HEX32_YELLOW      0xFFFF00FF
#define HEX32_CYAN        0x00FFFFFF
#define HEX32_MAGENTA     0xFF00FFFF
#define HEX32_ORANGE      0xFFA500FF
#define HEX32_LIME        0x32CD32FF
#define HEX32_PINK        0xFFC0CBFF
#define HEX32_PURPLE      0x800080FF
#define HEX32_VIOLET      0xEE82EEFF
#define HEX32_BROWN       0xA52A2AFF
#define HEX32_MAROON      0x800000FF
#define HEX32_NAVY        0x000080FF
#define HEX32_TEAL        0x008080FF
#define HEX32_OLIVE       0x808000FF
#define HEX32_INDIGO      0x4B0082FF
#define HEX32_CRIMSON     0xDC143CFF
#define HEX32_SKYBLUE     0x87CEEBFF
#define HEX32_TURQUOISE   0x40E0D0FF
#define HEX32_CORAL       0xFF7F50FF
#define HEX32_GOLD        0xFFD700FF
#define HEX32_KHAKI       0xF0E68CFF
#define HEX32_BEIGE       0xF5F5DCFF
#define HEX32_GRAY        0x808080FF
#define HEX32_DARKGRAY    0xA9A9A9FF
#define HEX32_LIGHTGRAY   0xD3D3D3FF
#define HEX32_SILVER      0xC0C0C0FF

#define RBGA(r,g,b,a) (r), (g), (b), (a)
#define HEX2RGB(hex)  ((hex)>>16), (((hex)>>8)&0xFF), ((hex)&0xFF), 255

#endif
