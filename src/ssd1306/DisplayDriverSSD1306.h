#pragma once

#include "DisplayDriver.h"

#include "ssd1306.h"


//! Abstract interface of display driver
class DisplayDriverSSD1306 : public DisplayDriver
{
public:
    //! Initialize display
    bool init() override;

    //! Get Display caracteristics
    int getWidth() override;
    int getHeight() override;

    //! Drawing commands
    void clear() override;
    void update() override;
    void drawLine(int x1, int y1, int x2, int y2) override;
    void drawPixel(int x, int y) override;

protected:
    void setup_gpios();

    SSD1306 m_display;
};