#pragma once
#include <stdint.h>

enum class Color : uint8_t { 
    BLACK = 0,
    GREY,
    WHITE,
    RED,
    YELLOW,
    GREEN,
    BLUE,
    BROWN,
    ORANGE,
    PURPLE,
    NB_COLORS
};

extern uint32_t ColorsRGB[];
