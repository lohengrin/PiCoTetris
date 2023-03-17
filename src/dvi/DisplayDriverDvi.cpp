#include "DisplayDriverDvi.h"

#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "hardware/clocks.h"
#include "hardware/irq.h"
#include "hardware/sync.h"
#include "hardware/gpio.h"
#include "hardware/vreg.h"

#include "dvi_serialiser.h"
#include "common_dvi_pin_configs.h"
#include "sprite.h"

#define FRAME_WIDTH 400
#define FRAME_HEIGHT 240
#define VREG_VSEL VREG_VOLTAGE_1_20
#define DVI_TIMING dvi_timing_640x480p_60hz

#define SW_A 14
#define SW_B 15
#define SW_C 16

using namespace pimoroni;

uint16_t *frameBuffer;

struct dvi_inst DisplayDriverDvi::dvi0;
pimoroni::PicoGraphics_PenRGB565 *DisplayDriverDvi::graphics = nullptr;

// Note first two scanlines are pushed before DVI start
volatile uint scanline = 2;

void core1_main()
{
    dvi_register_irqs_this_core(&DisplayDriverDvi::dvi0, DMA_IRQ_0);
    while (queue_is_empty(&DisplayDriverDvi::dvi0.q_colour_valid))
        __wfe();
    dvi_start(&DisplayDriverDvi::dvi0);
    dvi_scanbuf_main_16bpp(&DisplayDriverDvi::dvi0);
}

void core1_scanline_callback()
{
    // Discard any scanline pointers passed back
    uint16_t *bufptr;
    while (queue_try_remove_u32(&DisplayDriverDvi::dvi0.q_colour_free, &bufptr))
        ;
    bufptr = &frameBuffer[FRAME_WIDTH * scanline];
    queue_add_blocking_u32(&DisplayDriverDvi::dvi0.q_colour_valid, &bufptr);
    scanline = (scanline + 1) % FRAME_HEIGHT;
}

void vsync()
{
    while (scanline != 0)
    {
    };
}

DisplayDriverDvi::DisplayDriverDvi()
{
    frameBuffer = new uint16_t[FRAME_WIDTH*FRAME_HEIGHT];
    graphics = new pimoroni::PicoGraphics_PenRGB565 (FRAME_WIDTH, FRAME_HEIGHT, frameBuffer);
}

DisplayDriverDvi::~DisplayDriverDvi()
{
    delete graphics;
    delete[] frameBuffer;
}

bool DisplayDriverDvi::init()
{
    // graphic lib
    BG = graphics->create_pen(0, 0, 0);

    m_pens.resize(static_cast<int>(Color::NB_COLORS));

    for (int i = 0; i < static_cast<int>(Color::NB_COLORS); i++)
        m_pens[i] = graphics->create_pen(
            (ColorsRGB[i] >> 16) & 0xFF,
            (ColorsRGB[i] >> 8) & 0xFF,
            (ColorsRGB[i] >> 0) & 0xFF);

    // DVI
    vreg_set_voltage(VREG_VSEL);
    sleep_ms(10);
    set_sys_clock_khz(DVI_TIMING.bit_clk_khz, true);

    setup_default_uart();

    gpio_init(SW_A);
    gpio_set_dir(SW_A, GPIO_IN);
    gpio_pull_down(SW_A);
    gpio_init(SW_B);
    gpio_set_dir(SW_B, GPIO_IN);
    gpio_pull_down(SW_B);
    gpio_init(SW_C);
    gpio_set_dir(SW_C, GPIO_IN);
    gpio_pull_down(SW_C);

    dvi0.timing = &DVI_TIMING;
    dvi0.ser_cfg = DVI_DEFAULT_SERIAL_CONFIG;
    dvi0.scanline_callback = core1_scanline_callback;
    dvi_init(&dvi0, next_striped_spin_lock_num(), next_striped_spin_lock_num());

    sprite_fill16(frameBuffer, 0xffff, FRAME_WIDTH * FRAME_HEIGHT);
    uint16_t *bufptr = frameBuffer;
    queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);
    bufptr += FRAME_WIDTH;
    queue_add_blocking_u32(&dvi0.q_colour_valid, &bufptr);

    multicore_launch_core1(core1_main);

    return true;
}

//! Get Display caracteristics
int DisplayDriverDvi::getWidth()
{
    return FRAME_WIDTH;
}

int DisplayDriverDvi::getHeight()
{
    return FRAME_HEIGHT;
}

//! Drawing commands
void DisplayDriverDvi::clear()
{
    graphics->set_pen(BG);
    graphics->clear();
}

void DisplayDriverDvi::update()
{
    vsync();
    // update_buffer(ap, get_audio_frame);
    // f_read(&fil, (uint8_t *)&framebuf, FRAME_WIDTH * FRAME_HEIGHT * 2, &bytes_read);
}

void DisplayDriverDvi::setColor(Color color)
{
    graphics->set_pen(m_pens[static_cast<int>(color)]);
}

void DisplayDriverDvi::drawLine(int x1, int y1, int x2, int y2)
{
    Point p1(x1, y1);
    Point p2(x2, y2);
    graphics->line(p1, p2);
}

void DisplayDriverDvi::drawPixel(int x, int y)
{
    Point p1(x, y);
    graphics->pixel(p1);
}
