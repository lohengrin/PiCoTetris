#pragma once

#include "Game.h"

class Display {
    public:
        Display() {}
        virtual ~Display() {};

        virtual void draw(const Game& game) = 0;

};