#include "Piece.h"

#include "pico/stdlib.h"
#include "hardware/regs/rosc.h"
#include "hardware/regs/addressmap.h"

#include <algorithm>
#include <stdio.h>

uint32_t rnd(void)
{
    int k, random=0;
    volatile uint32_t *rnd_reg=(uint32_t *)(ROSC_BASE + ROSC_RANDOMBIT_OFFSET);
    
    for(k=0;k<32;k++){
        random = random << 1;
        random=random + (0x00000001 & (*rnd_reg));
    }
    return random;
}

Piece::Piece() 
{ 
    m_blocks.resize(4);
    for (auto&& l : m_blocks)
        l.resize(4,0);

    uint32_t rand = rnd();
    uint32_t type = rand % 7;

    printf("New Piece: %d %d\n", rand, type);

    switch (type) {
        case 0: // Square
            m_blocks[1][1] = 1;
            m_blocks[1][2] = 1;
            m_blocks[2][2] = 1;
            m_blocks[2][1] = 1;
            break;
        case 1: // Line
            m_blocks[0][2] = 1;
            m_blocks[1][2] = 1;
            m_blocks[2][2] = 1;
            m_blocks[3][2] = 1;
            break;
        case 2: // L
            m_blocks[0][1] = 1;
            m_blocks[1][1] = 1;
            m_blocks[2][1] = 1;
            m_blocks[2][2] = 1;
            break;
        case 3: // L invert
            m_blocks[0][2] = 1;
            m_blocks[1][2] = 1;
            m_blocks[2][2] = 1;
            m_blocks[2][1] = 1;
            break;
        case 4: // T
            m_blocks[0][2] = 1;
            m_blocks[1][2] = 1;
            m_blocks[2][2] = 1;
            m_blocks[1][1] = 1;
            break;
        case 5: // S
            m_blocks[0][2] = 1;
            m_blocks[1][2] = 1;
            m_blocks[1][1] = 1;
            m_blocks[2][1] = 1;
            break;
        case 6: // S invert
            m_blocks[0][1] = 1;
            m_blocks[1][1] = 1;
            m_blocks[1][2] = 1;
            m_blocks[2][2] = 1;
            break;
    };
}

void Piece::rotate()
{
    // Transpose
    for (int i = 0; i < 4; i++)
		for (int j = i; j < 4; j++)
			std::swap(m_blocks[i][j], m_blocks[j][i]);

    // Invert lines
    std::reverse(std::begin(m_blocks), std::end(m_blocks));
}
