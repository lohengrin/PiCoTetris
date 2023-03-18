#include "ControllerStdin.h"
#include "pico/stdlib.h"

ControllerStdin::ControllerStdin()
{
}

Controller::Command ControllerStdin::step()
{
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

    return Controller::NONE;
}
