#pragma once

#include "Controller.h"
#include "button.hpp"

class ControllerDvi : public Controller
{
public:
        ControllerDvi();

        Command step() override;

protected:
        // Buttons
        pimoroni::Button button_a;
        pimoroni::Button button_b;
        pimoroni::Button button_c;
};