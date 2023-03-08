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
    p_driver->clear();

    // Draw borders
    for (int i = 0; i < m_offsetBorder; i++)
    {
        p_driver->drawLine(0, i, 127, i);
        p_driver->drawLine(0, i + 51 + m_offsetBorder, 127, i + 51 + m_offsetBorder);
        p_driver->drawLine(i, 0, i, 51 + 2*m_offsetBorder);
    }

    // Draw Board
    for (size_t l = 0; l < game.height(); l++)
        for (size_t c = 0; c < game.width(); c++)
            if (game.board(l,c) == Game::Block::FILL)
                drawBlock(c,l);

    // Draw Piece
    auto piece = game.getPiece();
    auto pos = piece.getPos();

    for (int l = 0; l < 4; l++)
        for (int c = 0; c < 4; c++)
            if (piece.getBlock(l,c))
                drawBlock(pos.x+c,pos.y+l);


    p_driver->update();
}
