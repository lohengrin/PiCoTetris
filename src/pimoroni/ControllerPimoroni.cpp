
#include "ControllerPimoroni.h"
#include "pico/stdlib.h"

using namespace pimoroni;

const int repeat_time = 200;
const int hold_time = 400;

ControllerPimoroni::ControllerPimoroni() :
        button_a(PicoDisplay::A,Polarity::ACTIVE_LOW, repeat_time,hold_time),
        button_b(PicoDisplay::B,Polarity::ACTIVE_LOW, repeat_time,hold_time),
        button_x(PicoDisplay::X,Polarity::ACTIVE_LOW, repeat_time,hold_time),
        button_y(PicoDisplay::Y,Polarity::ACTIVE_LOW, repeat_time,hold_time)
{
}

Controller::Command ControllerPimoroni::step()
{
    if (button_x.raw() && button_y.raw()) return Controller::RESET;

    if (button_a.read()) return Controller::LEFT;
    if (button_b.read()) return Controller::RIGHT;
    if (button_x.read()) return Controller::DOWN;
    if (button_y.read()) return Controller::ROTATE;

    return Controller::NONE;
}
