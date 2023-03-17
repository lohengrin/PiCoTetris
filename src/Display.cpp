#include "Display.h"
#include "DisplayDriver.h"

Display::Display(DisplayDriver *driver, Orientation orient, const Game& game) : p_driver(driver)
{
    p_driver->init();

    m_width = p_driver->getWidth();
    m_height = p_driver->getHeight();

    //! Invert
    if (orient == PORTRAIT)
    {
        m_width = p_driver->getHeight();
        m_height = p_driver->getWidth();
    }

    m_blockSize = std::min(m_width/(game.width()+2), m_height/(game.height()+2));
    m_borderSize = 1;
    m_offsetBorder = m_blockSize;
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

    p_driver->setColor(Color::GREY);

    uint16_t offset = (game.width()+1)*m_blockSize + m_offsetBorder;
    // Draw borders
    for (int i = 0; i < m_offsetBorder; i++)
    {
        p_driver->drawLine(0, i, m_height, i);
        p_driver->drawLine(0, i + offset, m_height, i + offset);
        p_driver->drawLine(i, 0, i, offset);
    }

    // Draw Board
    for (int l = 0; l < game.height(); l++)
        for (int c = 0; c < game.width(); c++)
        {
            const Game::Block & b = game.board(l,c);
            if (b.type == Game::Block::FILL)
            {
                p_driver->setColor(b.color);
                drawBlock(c,l);
            }
        }

    // Draw Piece
    auto piece = game.getPiece();
    auto pos = piece.getPos();

    p_driver->setColor(piece.getColor());

    for (int l = 0; l < 4; l++)
        for (int c = 0; c < 4; c++)
            if (piece.getBlock(l,c))
                drawBlock(pos.x+c,pos.y+l);


    p_driver->update();
}
