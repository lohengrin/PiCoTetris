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

    Game(int width = 10, int height = 22);
    ~Game();

    //! Set controller command
    void setCommand(Controller::Command cmd);

    //! Step the game
    void step();

    //! Restart
    void reset();

    //! Board accessors
    Block::TYPE& board(int x, int y) { return m_board[x*m_width+y].type; }
    const Block::TYPE& board(int x, int y) const { return m_board[x*m_width+y].type; }
    int width() const { return m_width; }
    int height() const { return m_height; }

    //! get current piece
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
    Block *m_board;
    const int m_width;
    const int m_height;

    //! Current and next piece
    Piece m_currentPiece;
    Piece m_nextPiece;
    const Piece::Position m_InitPos = {3,19};

    //! Score
    uint32_t m_score = 0;
};