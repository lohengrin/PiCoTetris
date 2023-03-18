#pragma once

#include "Game.h"
#include "DisplayDriver.h"

class Display
{
public:



    enum Orientation { PORTRAIT, LANDSCAPE };

    Display(DisplayDriver *driver, Orientation orient, const Game& game);

    void draw(const Game &game);

protected:

    DisplayDriver::Point to(const DisplayDriver::Point& point) const;

    void drawBlock(uint8_t c, uint8_t l);

    DisplayDriver *p_driver;

    uint8_t m_blockSize = 4;
    uint8_t m_borderSize = 1;
    uint8_t m_offsetBorder = 7;
    int16_t m_width;
    int16_t m_height;

    const Orientation m_orient;
};