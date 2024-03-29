#include "Game.h"

#include <stdio.h>
#include <algorithm>
#include <functional>
#include <string.h>

//-----------------------------------------------------------------------
Game::Game(int width, int height) :
m_board(new Block[width*height]),
m_width(width),
m_height(height),
m_currentPiece(m_InitPos),
m_nextPiece(m_InitPos)
{
    reset();
}

//-----------------------------------------------------------------------
Game::~Game()
{
    delete[] m_board;
}

//-----------------------------------------------------------------------
void Game::setCommand(Controller::Command cmd)
{
    Piece::Position pos = m_currentPiece.getPos();

    switch(cmd)
    {
        case Controller::LEFT:
        {
            if (!(checkContact() & DirLeft))
            {
                pos.x -= 1;
                m_currentPiece.setPos(pos);
            }
        }
        break;
        case Controller::RIGHT:
        {
            if (!(checkContact() & DirRight))
            {
                pos.x += 1;
                m_currentPiece.setPos(pos);
            }
        }
        break;
        case Controller::DOWN:
        {
            if (!(checkContact() & DirBottom))
            {
                pos.y -= 1;
                m_currentPiece.setPos(pos);
            }
            else
                nextPiece();
        }
        break;
        case Controller::ROTATE:
        {
            auto tmp = m_currentPiece;
            m_currentPiece.rotate();
            if (checkCollision()) // Non valid rotation => go back
                m_currentPiece=tmp;
        }
        break;
        case Controller::NONE:
        case Controller::UNPAUSE:
        case Controller::RESET:
        default:
            break;
    };
}

//-----------------------------------------------------------------------
Game::GameStatus Game::step()
{
    static uint64_t count = 1;
    const int speed = 10;
    
    if (count++ % speed == 0)
    {
        if (checkContact() & DirBottom)
        {
            if (!nextPiece())
                return LOST;
            count = 1;
        }
        else
        {
            Piece::Position pos = m_currentPiece.getPos();
            pos.y -= 1;
            m_currentPiece.setPos(pos);
        }

        checkLines();
    }

    return RUNNING;
}

//-----------------------------------------------------------------------
void Game::reset()
{
    memset(m_board,0,m_width*m_height*sizeof(Block));

    m_currentPiece = Piece(m_InitPos);
    m_nextPiece = Piece(m_InitPos);

    m_score = 0;
}

//-----------------------------------------------------------------------
uint8_t Game::checkContact()
{
    auto && pos = m_currentPiece.getPos();

    uint8_t ret = DirNone;

    // Scan current piece to compute collision with border and board
    for (int l = 0; l < 4; l++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (m_currentPiece.getBlock(l,c))
            {
                int x = pos.x+c;
                int y = pos.y+l;

                if ((x == 0) || (board(y,x-1).fill)) 
                    ret = ret | DirLeft;
                if ((x == m_width-1) || (board(y,x+1).fill)) 
                    ret = ret | DirRight;
                if ((y == 0) || (board(y-1,x).fill))
                    ret = ret | DirBottom;
            }
        }
    }

    return ret;
}

//-----------------------------------------------------------------------
bool Game::nextPiece()
{
    auto && pos = m_currentPiece.getPos();

    for (int l = 0; l < 4; l++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (m_currentPiece.getBlock(l,c))
            {
                auto x = pos.x+c;
                auto y = pos.y+l;
                Block& b = board(y,x);
                b.fill = true;
                b.color = m_currentPiece.getColor();
            }
        }
    }

    m_currentPiece = m_nextPiece;
    m_nextPiece = Piece(m_InitPos);

    return !checkCollision(); // New piece collides => end game

}

bool Game::checkCollision()
{
    auto && pos = m_currentPiece.getPos();

    // Scan current piece to compute collision with border and board
    for (int l = 0; l < 4; l++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (m_currentPiece.getBlock(l,c))
            {
                int x = pos.x+c;
                int y = pos.y+l;

                if ((x < 0) || (x == m_width-1) || (board(y,x).fill)) 
                    return true;
            }
        }
    }

    return false;
}

//-----------------------------------------------------------------------
uint16_t Game::checkLines()
{
    std::vector<int> to_remove;

    // Search full lines accross board
    for (int l = 0; l < m_height; l++)
    {
        int count = 0;
        for (int c = 0; c < m_width; c++)
        {
            if (board(l,c).fill)
                count++;
        }

        if (count == m_width) // Full line
            to_remove.push_back(l);
    }

    // Remove full lines and shift other lines
    uint16_t score = 0;
    std::sort(to_remove.begin(), to_remove.end(), std::less<int>());
    int lineoffset = 0;
    for (auto i: to_remove)
    {
        // shift lines above the full line
        for (int l2 = i-lineoffset; l2 < m_height-1; l2++)
            for (int c = 0; c < m_width; c++)
                board(l2,c) = board(l2+1,c);

        // fill last line with empty line
        int lastLine = m_height-1;
        for (int c = 0; c < m_width; c++)
            board(lastLine,c).fill = false;

        lineoffset++;
        score += 10;
    }

    return score;
}
