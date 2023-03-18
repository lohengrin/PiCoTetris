#pragma once

#include "Colors.h"

#include <stdint.h>

class Piece
{
public:
    //! Position in game board coordinates
    struct Position
    {
        int8_t x = 0;
        int8_t y = 0;
    };

    Piece(const Position &pos);

    const Position &getPos() const { return m_pos; }
    void setPos(const Position &pos) { m_pos = pos; }
    void rotate();

    bool getBlock(int x, int y) const { return m_blocks[x][y]; }
    Color getColor() const { return m_color; }

protected:
    bool m_blocks[4][4];

    Position m_pos;
    Color m_color;
};
