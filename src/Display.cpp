#include "Display.h"
#include "DisplayDriver.h"

Display::Display(DisplayDriver *driver, Orientation orient) : p_driver(driver)
{
    p_driver->init();

    int width = p_driver->getWidth();
    int height = p_driver->getHeight();

    //! Invert
    if (orient == PORTRAIT)
    {
        width = p_driver->getHeight();
        height = p_driver->getWidth();
    }

    m_blockSize = 4;
    m_borderSize = 1;
    m_offsetBorder = 7;
}

void Display::drawBlock(uint8_t c, uint8_t l)
{
    for (int i = 0; i < m_blockSize; i++)
    {
        int x1 = m_offsetBorder + m_borderSize + l * (m_blockSize + m_borderSize) + i;
        int y1 = m_offsetBorder + m_borderSize + c * (m_blockSize + m_borderSize);
        p_driver->drawLine(x1, y1, x1, y1+m_blockSize-1);
    }
}

void Display::draw(const Game &game)
{
    auto board = game.getBoard();

    p_driver->clear();

    // Draw borders
    for (int i = 0; i < m_offsetBorder; i++)
    {
        p_driver->drawLine(0, i, 127, i);
        p_driver->drawLine(0, i + 51 + m_offsetBorder, 127, i + 51 + m_offsetBorder);
        p_driver->drawLine(i, 0, i, 51 + 2*m_offsetBorder);
    }

    // Draw Board
    for (size_t l = 0; l < board.size(); l++)
    {
        auto &&line = board[l];
        for (size_t c = 0; c < line.size(); c++)
        {
            if (line[c].type == Game::Block::FILL)
            {
                drawBlock(c,l);
            }
        }
    }

    // Draw Piece
    auto && piece = game.getPiece();
    auto && blocks = piece.getBlocks();
    auto && pos = piece.getPos();

    for (size_t l = 0; l < blocks.size(); l++)
    {
        auto &&line = blocks[l];
        for (size_t c = 0; c < line.size(); c++)
        {
            if (line[c] != 0)
            {
                drawBlock(pos.x+c,pos.y+l);
            }
        }
    }


    p_driver->update();
}
