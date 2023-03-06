#include "Piece.h"

Piece::Piece() 
{ 
    m_blocks.resize(4);
    for (auto&& l : m_blocks)
        l.resize(4);

    m_blocks[1][1] = 1;
    m_blocks[1][2] = 1;
    m_blocks[2][2] = 1;
    m_blocks[2][1] = 1;
}