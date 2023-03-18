#pragma once

#include "Colors.h"

//! Abstract interface of display driver
class DisplayDriver
{
public:
    struct Point
    {
        int16_t x = 0, y = 0;
    };

    virtual ~DisplayDriver() {}

    //! Initialize display
    virtual bool init() = 0;

    //! Get Display caracteristics
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    //! Drawing commands
    virtual void clear() = 0;
    virtual void update() = 0;

    virtual void setColor(const Color &color) = 0;
    virtual void drawLine(const Point &p1, const Point &p2) = 0;
    virtual void drawPixel(const Point &p) = 0;
};