#pragma once

#include <stdint.h>
#include <string.h>
#include <vector>

class Piece {
public:
    struct Position {
        int8_t x;
        int8_t y;   
    };

    typedef std::vector<uint8_t> Line;
    typedef std::vector<Line> Blocks;


    Piece();
    virtual ~Piece() {}

    const Position& getPos() const { return m_pos; }
    void setPos(const Position& pos ) { m_pos = pos; }

    void rotate();

    const Blocks& getBlocks() const { return m_blocks; }

protected:
    Blocks m_blocks;

    Position m_pos;
};
