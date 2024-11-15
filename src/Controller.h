#pragma once

class Controller {
public:
        enum Command {
            NOT_CONECTED = -1,
            NONE = 0,
            LEFT,
            RIGHT,
            DOWN,
            ROTATE,
            PAUSE,
            UNPAUSE,
            RESET
        };

        virtual Command step() = 0;
};