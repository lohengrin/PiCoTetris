#include "Game.h"

#include <stdio.h>
#include <algorithm>
#include <functional>

//-----------------------------------------------------------------------
Game::Game(int width, int height)
{
   m_board.resize(height);
    for (auto&& l : m_board)
        l.resize(width);

    reset();
}

//-----------------------------------------------------------------------
void Game::setCommand(Controller::Command cmd)
{
    Piece::Position pos = m_currentPiece.getPos();

    switch(cmd)
    {
        case Controller::LEFT:
        {
            if (!(checkCollision() & DirLeft))
            {
                pos.x -= 1;
                m_currentPiece.setPos(pos);
            }
        }
        break;
        case Controller::RIGHT:
        {
            if (!(checkCollision() & DirRight))
            {
                pos.x += 1;
                m_currentPiece.setPos(pos);
            }
        }
        break;
        case Controller::DOWN:
        {
            if (!(checkCollision() & DirBottom))
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
            m_currentPiece.rotate();
        }
        break;
    };
}

//-----------------------------------------------------------------------
void Game::step()
{
    static uint64_t count = 1;
    const int speed = 100;
    
    if (count++ % speed == 0)
    {
        if (checkCollision() & DirBottom)
        {
            nextPiece();
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
}

//-----------------------------------------------------------------------
void Game::reset()
{
    for ( auto&& c : m_board)
        for (auto&& l : c)
            l.type = Block::NONE;

    m_currentPiece.setPos(m_InitPos);
    m_nextPiece.setPos(m_InitPos);
}

//-----------------------------------------------------------------------
uint8_t Game::checkCollision()
{
    auto && blocks = m_currentPiece.getBlocks();
    auto && pos = m_currentPiece.getPos();

    uint8_t ret = DirNone;

    // Scan current piece to compute collision with border and board
    for (int l = 0; l < blocks.size(); l++)
    {
        auto &&line = blocks[l];
        for (int c = 0; c < line.size(); c++)
        {
            if (line[c] != 0)
            {
                int x = pos.x+c;
                int y = pos.y+l;

                if ((x == 0) || (m_board[y][x-1].type == Block::FILL)) 
                    ret = ret | DirLeft;
                if ((x == m_board[y].size()-1) || (m_board[y][x+1].type == Block::FILL)) 
                    ret = ret | DirRight;
                if ((y == 0) || (m_board[y-1][x].type == Block::FILL))
                    ret = ret | DirBottom;
            }
        }
    }

    return ret;
}

//-----------------------------------------------------------------------
void Game::nextPiece()
{
    auto && blocks = m_currentPiece.getBlocks();
    auto && pos = m_currentPiece.getPos();

    for (size_t l = 0; l < blocks.size(); l++)
    {
        auto &&line = blocks[l];
        for (size_t c = 0; c < line.size(); c++)
        {
            if (line[c] != 0)
            {
                auto x = pos.x+c;
                auto y = pos.y+l;

                m_board[y][x].type = Block::FILL;
            }
        }
    }

    m_currentPiece = m_nextPiece;
    m_nextPiece = Piece();
    m_nextPiece.setPos(m_InitPos);
}

//-----------------------------------------------------------------------
uint16_t Game::checkLines()
{
    std::vector<int> to_remove;

    for (int l = 0; l < m_board.size(); l++)
    {
        auto &&line = m_board[l];
        int count = 0;
        for (size_t c = 0; c < line.size(); c++)
        {
            if (line[c].type == Game::Block::FILL)
                count++;
        }

        if (count == line.size()) // Full line
        {
            to_remove.push_back(l);
        }
    }

    uint16_t score = 0;
    std::sort(to_remove.begin(), to_remove.end(), std::less<int>());
    for (auto i: to_remove)
    {
        printf("Full Line: %d \n", i);
        for (size_t l2 = i; l2 < m_board.size()-1; l2++)
            for (size_t c = 0; c < m_board[l2].size(); c++)
                m_board[l2][c] = m_board[l2+1][c];

        int lastLine = m_board.size()-1;
        for (size_t c = 0; c < m_board[lastLine].size(); c++)
            m_board[lastLine][c].type = Game::Block::NONE;

        score += 10;
    }

    return score;
}
