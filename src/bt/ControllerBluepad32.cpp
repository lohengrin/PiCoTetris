#include "ControllerBluepad32.h"
#include "pico/stdlib.h"

Bluepad32::GamepadPtr ControllerBluepad32::myGamepads[BP32_MAX_GAMEPADS];

ControllerBluepad32::ControllerBluepad32()
{
    memset(myGamepads,0, sizeof(myGamepads));
    // Setup the Bluepad32 callbacks
    Bluepad32::BP32.setup(&onConnectedGamepad, &onDisconnectedGamepad);
}

Controller::Command ControllerBluepad32::step()
{
    Bluepad32::BP32.update();

    // It is safe to always do this before using the gamepad API.
    // This guarantees that the gamepad is valid and connected.
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++)
    {
        Bluepad32::GamepadPtr myGamepad = myGamepads[i];

        if (myGamepad && myGamepad->isConnected())
        {
            // Another way to query the buttons, is by calling buttons(), or
            // miscButtons() which return a bitmask.
            // Some gamepads also have DPAD, axis and more.
            printf(
                "idx=%d, dpad: 0x%02x, buttons: 0x%04x, axis L: %4ld, %4ld, axis R: %4ld, "
                "%4ld, brake: %4ld, throttle: %4ld, misc: 0x%02x\n",
                i,                       // Gamepad Index
                myGamepad->dpad(),       // DPAD
                myGamepad->buttons(),    // bitmask of pressed buttons
                myGamepad->axisX(),      // (-511 - 512) left X Axis
                myGamepad->axisY(),      // (-511 - 512) left Y axis
                myGamepad->axisRX(),     // (-511 - 512) right X axis
                myGamepad->axisRY(),     // (-511 - 512) right Y axis
                myGamepad->brake(),      // (0 - 1023): brake button
                myGamepad->throttle(),   // (0 - 1023): throttle (AKA gas) button
                myGamepad->miscButtons() // bitmak of pressed "misc" buttons
            );

            // You can query the axis and other properties as well. See Gamepad.h
            // For all the available functions.
        }
    }
#if 0
    int c = getchar_timeout_us(0);
    switch (c)
    {
    case PICO_ERROR_TIMEOUT:
        return Controller::NONE;
    case 'q':
        return Controller::LEFT;
    case 'd':
        return Controller::RIGHT;
    case 's':
        return Controller::DOWN;
    case ' ':
        return Controller::ROTATE;
    case 'r':
        return Controller::RESET;

    default:
        return Controller::NONE;
    }
#endif
    return Controller::NONE;
}

// This callback gets called any time a new gamepad is connected.
// Up to 4 gamepads can be connected at the same time.
void ControllerBluepad32::onConnectedGamepad(Bluepad32::GamepadPtr gp)
{
    bool foundEmptySlot = false;
    for (int i = 0; i < BP32_MAX_GAMEPADS; i++)
    {
        if (myGamepads[i] == nullptr)
        {
            printf("CALLBACK: Gamepad is connected, index=%d\n", i);
            // Additionally, you can get certain gamepad properties like:
            // Model, VID, PID, BTAddr, flags, etc.
            Bluepad32::GamepadProperties properties = gp->getProperties();
            printf("Gamepad model: %s, VID=0x%04x, PID=0x%04x\n", gp->getModelName().c_str(), properties.vendor_id,
                   properties.product_id);
            myGamepads[i] = gp;
            foundEmptySlot = true;
            break;
        }
    }
    if (!foundEmptySlot)
    {
        printf("CALLBACK: Gamepad connected, but could not found empty slot\n");
    }
}

void ControllerBluepad32::onDisconnectedGamepad(Bluepad32::GamepadPtr gp)
{
    bool foundGamepad = false;

    for (int i = 0; i < BP32_MAX_GAMEPADS; i++)
    {
        if (myGamepads[i] == gp)
        {
            printf("CALLBACK: Gamepad is disconnected from index=%d\n", i);
            myGamepads[i] = nullptr;
            foundGamepad = true;
            break;
        }
    }

    if (!foundGamepad)
    {
        printf("CALLBACK: Gamepad disconnected, but not found in myGamepads\n");
    }
}