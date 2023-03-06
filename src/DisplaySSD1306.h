#pragma once

#include "Display.h"
#include "ssd1306.h"

class DisplaySSD1306 : public Display {
public:
    DisplaySSD1306();

    virtual void draw(const Game& game);

protected:
    void setup_gpios();
    void drawBlock(uint8_t c, uint8_t l);

    SSD1306 m_display;

    const uint8_t m_blockSize = 4;
    const uint8_t m_borderSize = 1;
    const uint8_t m_offsetBorder = 7;

};