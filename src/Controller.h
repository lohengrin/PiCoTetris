#pragma once

class Controller {
public:
        enum Command {
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