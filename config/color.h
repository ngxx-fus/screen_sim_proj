#ifndef __COLOR_H__
#define __COLOR_H__

#define BLACK       0,      0,      0,      255
#define WHITE       255,    255,    255,    255
#define RED         255,    0,      0,      255
#define GREEN       0,      255,    0,      255
#define BLUE        0,      0,      255,    255
#define YELLOW      255,    255,    0,      255
#define CYAN        0,      255,    255,    255
#define MAGENTA     255,    0,      255,    255
#define ORANGE      255,    165,    0,      255
#define LIME        50,     205,    50,     255
#define PINK        255,    192,    203,    255
#define PURPLE      128,    0,      128,    255
#define VIOLET      238,    130,    238,    255
#define BROWN       165,    42,     42,     255
#define MAROON      128,    0,      0,      255
#define NAVY        0,      0,      128,    255
#define TEAL        0,      128,    128,    255
#define OLIVE       128,    128,    0,      255
#define INDIGO      75,     0,      130,    255
#define CRIMSON     220,    20,     60,     255
#define SKYBLUE     135,    206,    235,    255
#define TURQUOISE   64,     224,    208,    255
#define CORAL       255,    127,    80,     255
#define GOLD        255,    215,    0,      255
#define KHAKI       240,    230,    140,    255
#define BEIGE       245,    245,    220,    255
#define GRAY        128,    128,    128,    255
#define DARKGRAY    169,    169,    169,    255
#define LIGHTGRAY   211,    211,    211,    255
#define SILVER      192,    192,    192,    255

#define RBGA(r,g,b,a) (r), (g), (b), (a)
#define HEX2RGB(hex)  ((hex)>>16), (((hex)>>8)&0xFF), ((hex)&0xFF), 255

#endif