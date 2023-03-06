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

void DisplayDriverSSD1306::drawLine(int x1, int y1, int x2, int y2)
{
    m_display.draw_line(x1, y1, x2, y2);
}

void DisplayDriverSSD1306::setup_gpios()
{
    printf("configuring pins...\n");
    i2c_init(i2c1, 400000);
    gpio_set_function(19, GPIO_FUNC_I2C);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_pull_up(19);
    gpio_pull_up(18);
}