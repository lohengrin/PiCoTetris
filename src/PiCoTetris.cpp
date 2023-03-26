
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

#ifdef CONTROL_BT
	#include "pico/cyw43_arch.h"
	#include "ControllerBluepad32.h"
	#include "btstack.h"
	#include "uni_main.h"

	static btstack_timer_source_t work_timer;
#endif

#include <memory>
#include <stdio.h>

#define PERIOD_US 100000 // 10 Hz

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

//----------------------------------------------------------
void gameStep(GameContext *gc)
{
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
}

#ifdef CONTROL_BT
static void work_timer_handler(btstack_timer_source_t *ts)
{
	GameContext *gc = reinterpret_cast<GameContext *>(ts->context);

	gameStep(gc);

	// set timer for next tick
	btstack_run_loop_set_timer(&work_timer, 100);
	btstack_run_loop_add_timer(&work_timer);
}
#endif

int main()
{
	stdio_init_all();

	// Selected controller
	Controller * ctrl = nullptr;
	DisplayDriver * dispDrv = nullptr;
	Display::Orientation orientation = Display::PORTRAIT;

#ifdef CONTROL_BT
    // initialize CYW43 driver architecture (will enable BT if/because CYW43_ENABLE_BLUETOOTH == 1)
    if (cyw43_arch_init()) {
        printf("failed to initialise cyw43_arch\n");
        return -1;
    }

	ctrl = new ControllerBluepad32;
#elif defined(PIMORONI)
	ctrl = new ControllerPimoroni;
#else
	ctrl = new ControllerStdin;
#endif

// Display selection at compilation time
#ifdef PIMORONI
	dispDrv = new DisplayDriverPimoroni();

	// pimoroni::RGBLED led(pimoroni::PicoDisplay::LED_R, pimoroni::PicoDisplay::LED_G, pimoroni::PicoDisplay::LED_B);
	// led.set_rgb(0, 50, 0);
#elif defined(SSD1306)
	dispDrv = new DisplayDriverSSD1306();
#elif  defined(DVI)
	dispDrv = new DisplayDriverDvi();
	orientation = Display::LANDSCAPE;
#endif

	if (!ctrl || !dispDrv)
	{
		printf("Invalid configuration.\n");
		exit(1);
	}

	GameContext gameCtx(ctrl, dispDrv, orientation, 10, 22);


#ifdef CONTROL_BT
	// set timer for workload
	btstack_run_loop_set_timer_context(&work_timer, &gameCtx);
	btstack_run_loop_set_timer_handler(&work_timer, work_timer_handler);
	btstack_run_loop_set_timer(&work_timer, 100);
	btstack_run_loop_add_timer(&work_timer);

	uni_main(0, NULL);
#else
	while(true)
	{
		gameStep(&gameCtx);
		sleep_ms(10);
	}
#endif

	return 0;
}
