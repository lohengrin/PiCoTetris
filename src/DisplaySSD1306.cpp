#include "DisplaySSD1306.h"

#include <stdio.h>

DisplaySSD1306::DisplaySSD1306() : Display()
{
    // Init Display
    setup_gpios();

    m_display.m_external_vcc = false;
    m_display.init(128, 64, 0x3C, i2c1);
    m_display.clear();
}

void DisplaySSD1306::drawBlock(uint8_t c, uint8_t l)
{
    for (int i = 0; i < m_blockSize; i++)
    {
        int x1 = m_offsetBorder + m_borderSize + l * (m_blockSize + m_borderSize) + i;
        int y1 = m_offsetBorder + m_borderSize + c * (m_blockSize + m_borderSize);
        m_display.draw_line(x1, y1, x1, y1+m_blockSize-1);
    }

}

void DisplaySSD1306::draw(const Game &game)
{
    auto board = game.getBoard();

    m_display.clear();

    // Draw borders
    for (int i = 0; i < m_offsetBorder; i++)
    {
        m_display.draw_line(0, i, 127, i);
        m_display.draw_line(0, i + 51 + m_offsetBorder, 127, i + 51 + m_offsetBorder);
        m_display.draw_line(i, 0, i, 51 + 2*m_offsetBorder);
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


    m_display.show();
}

void DisplaySSD1306::setup_gpios()
{
    printf("configuring pins...\n");
    i2c_init(i2c1, 400000);
    gpio_set_function(19, GPIO_FUNC_I2C);
    gpio_set_function(18, GPIO_FUNC_I2C);
    gpio_pull_up(19);
    gpio_pull_up(18);
}