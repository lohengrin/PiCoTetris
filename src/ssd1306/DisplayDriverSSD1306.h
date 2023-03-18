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


    void drawLine(const Point& p1, const Point& p2) override;
    void drawPixel(const Point& p) override;
protected:
    void setup_gpios();

    SSD1306 m_display;
};