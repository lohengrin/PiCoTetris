
#include "ControllerDvi.h"
#include "pico/stdlib.h"

using namespace pimoroni;

#define SW_A 14
#define SW_B 15
#define SW_C 16

const int repeat_time = 200;
const int hold_time = 400;

ControllerDvi::ControllerDvi() : button_a(SW_A, Polarity::ACTIVE_HIGH, repeat_time, hold_time),
                                 button_b(SW_B, Polarity::ACTIVE_HIGH, repeat_time, hold_time),
                                 button_c(SW_C, Polarity::ACTIVE_HIGH, repeat_time, hold_time)
{
}

Controller::Command ControllerDvi::step()
{
    if (button_a.read())
        return Controller::LEFT;
    if (button_c.read())
        return Controller::RIGHT;
    if (button_b.read())
        return Controller::ROTATE;

    return Controller::NONE;
}
