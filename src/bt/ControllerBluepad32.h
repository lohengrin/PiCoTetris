#pragma once

#include "Controller.h"
#include "Bluepad32.h"

class ControllerBluepad32 : public Controller
{
public:
        ControllerBluepad32();

        Command step() override;

        static void onConnectedGamepad(Bluepad32::GamepadPtr gp);
        static void onDisconnectedGamepad(Bluepad32::GamepadPtr gp);
        static Bluepad32::GamepadPtr myGamepads[BP32_MAX_GAMEPADS];
};