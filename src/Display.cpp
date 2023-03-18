#include "Display.h"
#include "DisplayDriver.h"

Display::Display(DisplayDriver *driver, Orientation orient, const Game& game) : p_driver(driver), m_orient(orient)
{
    p_driver->init();

    m_width = p_driver->getWidth();
    m_height = p_driver->getHeight();

    //! Invert
    if (m_orient == PORTRAIT)
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
        DisplayDriver::Point p1, p2;
        p2.x = p1.x = m_offsetBorder + m_borderSize + l * (m_blockSize + m_borderSize) + i;
        p1.y = m_offsetBorder + m_borderSize + c * (m_blockSize + m_borderSize);
        p2.y = p1.y + m_blockSize-1;
        p_driver->drawLine(to(p1), to(p2));
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
        p_driver->drawLine(to({0,i}), to({m_height, i}));
        p_driver->drawLine(to({0,i + offset}), to({m_height, i + offset}));
        p_driver->drawLine(to({i, 0}), to({i, offset}));
    }

    // Draw Board
    for (int l = 0; l < game.height(); l++)
        for (int c = 0; c < game.width(); c++)
        {
            const Game::Block & b = game.board(l,c);
            if (b.fill)
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


DisplayDriver::Point Display::to(const DisplayDriver::Point& point) const
{
    switch (m_orient)
    {
        case LANDSCAPE:
            return {point.y, m_height-point.x};
        default:
        case PORTRAIT:
            return point;
    };
}
