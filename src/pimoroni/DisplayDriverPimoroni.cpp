#include "DisplayDriverPimoroni.h"

#include <stdio.h>

using namespace pimoroni;


DisplayDriverPimoroni::DisplayDriverPimoroni() :
    st7789(PicoDisplay::WIDTH, PicoDisplay::HEIGHT, ROTATE_0, false, get_spi_pins(BG_SPI_FRONT)),
    graphics(st7789.width, st7789.height, nullptr)
{

}

bool DisplayDriverPimoroni::init()
{
    BG = graphics.create_pen(0,0,0);

    m_pens.resize(static_cast<int>(Color::NB_COLORS));

    for (int i = 0; i < static_cast<int>(Color::NB_COLORS); i++)
        m_pens[i] = graphics.create_pen(
            (ColorsRGB[i] >> 16) & 0xFF,
            (ColorsRGB[i] >> 8) & 0xFF,
            (ColorsRGB[i] >> 0) & 0xFF
            );
    return true;
}

//! Get Display caracteristics
int DisplayDriverPimoroni::getWidth()
{
    return PicoDisplay::WIDTH;
}

int DisplayDriverPimoroni::getHeight()
{
    return PicoDisplay::HEIGHT;
}

//! Drawing commands
void DisplayDriverPimoroni::clear()
{
    graphics.set_pen(BG);
    graphics.clear();
}

void DisplayDriverPimoroni::update()
{
    // update screen
    st7789.update(&graphics);
}

void DisplayDriverPimoroni::setColor(const Color& color)
{
    graphics.set_pen(m_pens[static_cast<int>(color)]);
}

void DisplayDriverPimoroni::drawLine(const DisplayDriver::Point& p1, const DisplayDriver::Point& p2)
{
    pimoroni::Point pp1(p1.x,p1.y);
    pimoroni::Point pp2(p2.x,p2.y);
    graphics.line(pp1,pp2);
}

void DisplayDriverPimoroni::drawPixel(const DisplayDriver::Point& p)
{
    pimoroni::Point p1(p.x,p.y);
    graphics.pixel(p1);
}
