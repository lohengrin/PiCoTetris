#pragma once

#include "DisplayDriver.h"

#include "dvi.h"
#include "libraries/pico_graphics/pico_graphics.hpp"

//! Abstract interface of display driver
class DisplayDriverDvi : public DisplayDriver
{
public:
    DisplayDriverDvi();
    ~DisplayDriverDvi();

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

    static struct dvi_inst dvi0;

protected:
    int frontb = 0;
    int backb = 1;
    pimoroni::PicoGraphics_PenRGB332 graphics;
    pimoroni::Pen BG; // Background pen
    std::vector<pimoroni::Pen> m_pens; // Colors pens
};
