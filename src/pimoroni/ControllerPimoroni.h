#pragma once

#include "Controller.h"

#include "pico_display.hpp"
#include "button.hpp"

class ControllerPimoroni : public Controller
{
public:
        ControllerPimoroni();

        Command step() override;

protected:
        // Buttons
        pimoroni::Button button_a;
        pimoroni::Button button_b;
        pimoroni::Button button_x;
        pimoroni::Button button_y;
};