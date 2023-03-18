#pragma once

#include "Controller.h"


class ControllerDvi : public Controller
{
public:
        ControllerDvi();

        Command step() override;

protected:
};