#pragma once

#include "Game.h"

class DisplayDriver;

class Display
{
public:
    Display(DisplayDriver *driver);

    void draw(const Game &game);

protected:
    void drawBlock(uint8_t c, uint8_t l);

    DisplayDriver *p_driver;

    const uint8_t m_blockSize = 4;
    const uint8_t m_borderSize = 1;
    const uint8_t m_offsetBorder = 7;
};