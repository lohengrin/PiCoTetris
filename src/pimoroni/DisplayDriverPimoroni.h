#pragma once

#include "DisplayDriver.h"

#include "pico_display.hpp"
#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

//! Abstract interface of display driver
class DisplayDriverPimoroni : public DisplayDriver
{
public:
    DisplayDriverPimoroni();

    //! Initialize display
    bool init() override;

    //! Get Display caracteristics
    int getWidth() override;
    int getHeight() override;

    //! Drawing commands
    void clear() override;
    void update() override;
    void setColor(Color color);
    void drawLine(int x1, int y1, int x2, int y2) override;
    void drawPixel(int x, int y) override;

protected:
    pimoroni::ST7789 st7789;
    pimoroni::PicoGraphics_PenRGB565 graphics;

    pimoroni::Pen BG; // Background pen
    std::vector<pimoroni::Pen> m_pens; // Colors pens
};