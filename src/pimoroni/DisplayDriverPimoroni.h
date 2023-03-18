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

    void setColor(const Color& color) override;
    void drawLine(const Point& p1, const Point& p2) override;
    void drawPixel(const Point& p) override;

protected:
    pimoroni::ST7789 st7789;
    pimoroni::PicoGraphics_PenRGB565 graphics;

    pimoroni::Pen BG; // Background pen
    std::vector<pimoroni::Pen> m_pens; // Colors pens
};