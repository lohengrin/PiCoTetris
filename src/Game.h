#pragma once

#include "Piece.h"
#include "Controller.h"

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

    void setCommand(Controller::Command cmd);

    void step();

    void reset();

    const Board& getBoard() const { return m_board; }
    const Piece& getPiece() const { return m_currentPiece; }

private:
    const uint8_t DirNone   = 0x00;
    const uint8_t DirBottom = 0x01;
    const uint8_t DirLeft   = 0x02;
    const uint8_t DirRight  = 0x04;

    // Return true if piece land on another block or bottom
    uint8_t checkCollision();
    uint16_t checkLines();
    void nextPiece();

    Board m_board;

    Piece m_currentPiece;
    Piece m_nextPiece;
    const Piece::Position m_InitPos = {3,20};

};