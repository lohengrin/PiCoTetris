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

    //! Set controller command
    void setCommand(Controller::Command cmd);

    //! Step the game
    void step();

    //! Restart
    void reset();

    //! Accessor for Display
    const Board& getBoard() const { return m_board; }
    const Piece& getPiece() const { return m_currentPiece; }

private:
    //! Collision Directions
    const uint8_t DirNone   = 0x00;
    const uint8_t DirBottom = 0x01;
    const uint8_t DirLeft   = 0x02;
    const uint8_t DirRight  = 0x04;

    //! Return if piece collide in one or more directions
    uint8_t checkCollision();

    //! Check if a line is completed
    uint16_t checkLines();

    //! Switch to next Piece
    void nextPiece();

    //! The game board
    Board m_board;

    //! Current and next piece
    Piece m_currentPiece;
    Piece m_nextPiece;
    const Piece::Position m_InitPos = {3,19};

    //! Score
    uint32_t m_score = 0;
};