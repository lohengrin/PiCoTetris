#pragma once

#include <stdint.h>
#include <string.h>
#include <vector>

class Piece {
public:
    struct Position {
        int8_t x=0;
        int8_t y=0;   
    };

    Piece(const Position& pos);

    const Position& getPos() const { return m_pos; }
    void setPos(const Position& pos ) { m_pos = pos; }

    void rotate();

    bool getBlock(int x, int y) const { return m_blocks[x][y]; }

protected:
    bool m_blocks[4][4];

    Position m_pos;
};
