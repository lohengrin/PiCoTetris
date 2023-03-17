#pragma once

#include "Piece.h"
#include "Controller.h"

#include <stdint.h>
#include <vector>

class Game {
public:
    enum GameStatus {
        RUNNING,
        LOST
    };

    struct Block {
        Block() : fill(false), color(Color::WHITE) {}
        bool fill;
        Color color;
    };

    Game(int width = 10, int height = 22);
    ~Game();

    //! Set controller command
    void setCommand(Controller::Command cmd);

    //! Step the game
    GameStatus step();

    //! Restart
    void reset();

    //! Board accessors
    Block& board(int x, int y) { return m_board[x*m_width+y]; }
    const Block& board(int x, int y) const { return m_board[x*m_width+y]; }
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

    //! Return if piece is in contact in one or more directions
    uint8_t checkContact();

    //! Return true if the piece collide with other blocks or borders
    //! Used to avoid non valid rotation or detect end game
    bool checkCollision();

    //! Check if a line is completed
    uint16_t checkLines();

    //! Switch to next Piece
    //! return false if new piece collide
    bool nextPiece();

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