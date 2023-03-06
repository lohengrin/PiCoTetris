#include "Game.h"

#include <stdio.h>

Game::Game(int width, int height)
{
   m_board.resize(height);
    for (auto&& l : m_board)
        l.resize(width);

    reset();
}

void Game::step()
{
    static uint64_t count = 1;
    
    if (count++ % 100 == 0)
    {

        if (checkLanding())
        {
            nextPiece();
            count = 1;
            printf("Landing\n");
        }
        else
        {
            Piece::Position pos = m_currentPiece.getPos();
            pos.y -= 1;
            m_currentPiece.setPos(pos);
        }
    }
}

void Game::reset()
{
    for ( auto&& c : m_board)
        for (auto&& l : c)
            l.type = Block::NONE;

//    m_board[5][3].type = Block::FILL;
//    m_board[5][2].type = Block::FILL;
//    m_board[12][6].type = Block::FILL;
//    m_board[1][9].type = Block::FILL;

    m_currentPiece.setPos(m_InitPos);
    m_nextPiece.setPos(m_InitPos);
}

bool Game::checkLanding()
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
                auto x = pos.x+l;
                auto y = pos.y+c;
                if (y == 0) return true;
                else if (m_board[y-1][x].type == Block::FILL) return true;
            }
        }
    }

    return false;
}

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
    m_nextPiece.setPos(m_InitPos);
}
