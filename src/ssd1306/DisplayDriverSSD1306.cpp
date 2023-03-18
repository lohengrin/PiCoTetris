#include "DisplayDriverSSD1306.h"

#include <stdio.h>

bool DisplayDriverSSD1306::init()
{
    setup_gpios();

    m_display.m_external_vcc = false;
    m_display.init(128, 64, 0x3C, i2c1);
    m_display.clear();

    return true;
}

//! Get Display caracteristics
int DisplayDriverSSD1306::getWidth()
{
    return 128;
}

int DisplayDriverSSD1306::getHeight()
{
    return 64;
}

//! Drawing commands
void DisplayDriverSSD1306::clear()
{
    m_display.clear();
}

void DisplayDriverSSD1306::update()
{
    m_display.show();
}

void DisplayDriverSSD1306::drawLine(const DisplayDriver::Point& p1, const DisplayDriver::Point& p2)
{
    m_display.draw_line(p1.x, p1.y, p2.x, p2.y);
}

void DisplayDriverSSD1306::drawPixel(const DisplayDriver::Point& p)
{
    m_display.draw_pixel(p.x, p.y);
}

void DisplayDriverSSD1306::setup_gpios()
{
    printf("configuring pins...\n");
    i2c_init(i2c1, 800000);
    gpio_set_function(19, GPIO_FUNC_I2C);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_pull_up(19);
    gpio_pull_up(18);
}