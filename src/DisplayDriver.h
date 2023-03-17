#pragma once

#include "Colors.h"

//! Abstract interface of display driver
class DisplayDriver {
public:
    virtual ~DisplayDriver() {}

    //! Initialize display
    virtual bool init() = 0;

    //! Get Display caracteristics
    virtual int getWidth() = 0;
    virtual int getHeight() = 0;

    //! Drawing commands
    virtual void clear() = 0;
    virtual void update() = 0;

    virtual void setColor(Color color) = 0;
    virtual void drawLine(int x1, int y1, int x2, int y2) = 0;
    virtual void drawPixel(int x, int y) = 0;
};