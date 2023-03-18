#pragma once

#include "Controller.h"

class ControllerStdin : public Controller
{
public:
        ControllerStdin();

        Command step() override;
};