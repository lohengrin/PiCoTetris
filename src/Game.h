#pragma once

#include "Piece.h"

#include <stdint.h>
#include <vector>

class Game {
public:
    struct Block {
        enum TYPE {
            NONE = 0x00,
            FILL = 0x01
        };
        Block() : type(NONE) {}
        TYPE type;
    };

    typedef std::vector<Block> Line;
    typedef std::vector<Line> Board;

    Game(int width = 10, int height = 22);
    ~Game() {}

    void step();

    void reset();

    const Board& getBoard() const { return m_board; }
    const Piece& getPiece() const { return m_currentPiece; }

private:
    // Return true if piece land on another block or bottom
    bool checkLanding();
    void nextPiece();

    Board m_board;

    Piece m_currentPiece;
    Piece m_nextPiece;
    const Piece::Position m_InitPos = {3,10};

};