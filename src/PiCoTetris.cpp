
/*******************************************************************/
/* PICO Based Tetris like game                                     */
/* Guillaume Millet												   */
/*******************************************************************/

#include "Game.h"
#include "Display.h"
#include "Controller.h"
#include "pico/stdlib.h"

//! STDIN Controller
#include "ControllerStdin.h"

#ifdef DVI
//! DVI Driver
#include "dvi/DisplayDriverDvi.h"
#include "dvi/ControllerDvi.h"
#endif

#ifdef SSD1306
//! SSD1306 Driver
#include "ssd1306/DisplayDriverSSD1306.h"
#endif

#ifdef PIMORONI
//! PIMORONI Driver
#include "pimoroni/DisplayDriverPimoroni.h"
#include "pimoroni/ControllerPimoroni.h"
#include "rgbled.hpp"
#endif

// PICO SDK
#include "pico/stdlib.h"

#ifdef RASPBERRYPI_PICO_W
#include "pico/cyw43_arch.h"
#include "picow_bt.h"
#include "btstack.h"
#endif

#include <memory>
#include <stdio.h>

#define PERIOD_US 100000 // 10 Hz
static btstack_timer_source_t work_timer;

struct GameContext
{
	GameContext(
		Controller *ctrl,
		DisplayDriver *dd,
		Display::Orientation orient,
		int width, int height) : m_game(width, height),
								 m_controller(ctrl),
								 m_disp_driver(dd),
								 m_disp(m_disp_driver.get(), orient, m_game),
								 m_status(Game::GameStatus::RUNNING)
	{
	}
	Game m_game;
	std::unique_ptr<Controller> m_controller;
	std::unique_ptr<DisplayDriver> m_disp_driver;
	Display m_disp;
	Game::GameStatus m_status;
};

static void work_timer_handler(btstack_timer_source_t *ts)
{
	GameContext *gc = reinterpret_cast<GameContext *>(ts->context);

	// Get joystick event
	Controller::Command cmd = gc->m_controller->step();


	if (gc->m_status == Game::GameStatus::RUNNING)
	{
		gc->m_game.setCommand(cmd);

		// Step Game
		gc->m_status = gc->m_game.step();

		// Display Game
		gc->m_disp.draw(gc->m_game);
	}
	else
	{
		if (cmd == Controller::Command::RESET)
		{
			gc->m_game.reset();
			gc->m_status = Game::GameStatus::RUNNING;
		}
		sleep_ms(100);
	}
	// set timer for next tick
//	btstack_run_loop_set_timer_context(&work_timer, gc);
//	btstack_run_loop_set_timer_handler(&work_timer, work_timer_handler);
//	btstack_run_loop_set_timer(&work_timer, 10);
//	btstack_run_loop_add_timer(&work_timer);
}

int main()
{
	stdio_init_all();

// Display selection at compilation time
#ifdef PIMORONI
	// pimoroni::RGBLED led(pimoroni::PicoDisplay::LED_R, pimoroni::PicoDisplay::LED_G, pimoroni::PicoDisplay::LED_B);
	// led.set_rgb(0, 50, 0);

	GameContext gameCtx(
		new ControllerPimoroni(),
		new DisplayDriverPimoroni(),
		Display::PORTRAIT,
		10, 22);
#endif
#ifdef SSD1306
	GameContext gameCtx(
		new ControllerStdin(),
		new DisplayDriverSSD1306(),
		Display::PORTRAIT,
		10, 22);
#endif
#ifdef DVI
	GameContext gameCtx(
		new ControllerDvi(),
		new DisplayDriverDvi(),
		Display::LANDSCAPE,
		10, 22);
#endif

	// set timer for workload
	btstack_run_loop_set_timer_context(&work_timer, &gameCtx);
	btstack_run_loop_set_timer_handler(&work_timer, work_timer_handler);
	btstack_run_loop_set_timer(&work_timer, 10);
	btstack_run_loop_add_timer(&work_timer);

	picow_bt_main();
	btstack_run_loop_execute();
/*
	btstack_timer_source_t ts;
	ts.context = &gameCtx;

	while(true)
	{
		work_timer_handler(&ts);
		sleep_ms(10);
	}
*/

	return 0;
}
