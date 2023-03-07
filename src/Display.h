#pragma once

#include "Game.h"

class DisplayDriver;

class Display
{
public:
    enum Orientation { LANDSCAPE, PORTRAIT };

    Display(DisplayDriver *driver, Orientation orient);

    void draw(const Game &game);

protected:
    void drawBlock(uint8_t c, uint8_t l);

    DisplayDriver *p_driver;

    uint8_t m_blockSize = 4;
    uint8_t m_borderSize = 1;
    uint8_t m_offsetBorder = 7;
};