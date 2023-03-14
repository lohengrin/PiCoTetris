#pragma once

#include "Controller.h"

class ControllerPimoroni : public Controller {
public:
        ControllerPimoroni();

        Command step() override;
};