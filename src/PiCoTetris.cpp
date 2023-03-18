
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

#include <memory>
#include <stdio.h>

#define PERIOD_US 100000 // 10 Hz

int main()
{
	stdio_init_all();

	while (true)
	{
		Game game(10, 22);

// Display selection at compilation time
#ifdef PIMORONI
		pimoroni::RGBLED led(pimoroni::PicoDisplay::LED_R, pimoroni::PicoDisplay::LED_G, pimoroni::PicoDisplay::LED_B);
		led.set_rgb(0, 50, 0);

		std::unique_ptr<Controller> controller(new ControllerPimoroni);
		std::unique_ptr<DisplayDriver> disp_driver(new DisplayDriverPimoroni);
		Display disp(disp_driver.get(), Display::PORTRAIT, game);
#endif
#ifdef SSD1306
		std::unique_ptr<Controller> controller(new ControllerStdin);
		std::unique_ptr<DisplayDriver> disp_driver(new DisplayDriverSSD1306);
		Display disp(disp_driver.get(), Display::PORTRAIT, game);
#endif
#ifdef DVI
		std::unique_ptr<Controller> controller(new ControllerStdin);
		std::unique_ptr<DisplayDriver> disp_driver(new DisplayDriverDvi);
		Display disp(disp_driver.get(), Display::LANDSCAPE, game);
#endif


		//! Scheduling
		absolute_time_t nextStep = delayed_by_us(get_absolute_time(), PERIOD_US);

		//! Current game status
		Game::GameStatus ret = Game::RUNNING;
		Controller::Command cmd = Controller::NONE;

		//! Game main loop
		while ((ret == Game::RUNNING) && (cmd != Controller::RESET))
		{
			// Get joystick event
			cmd = controller->step();

			game.setCommand(cmd);

			// Step Game
			ret = game.step();

			// Display Game
			disp.draw(game);

			// Wait next step according to PERIOD_US
			busy_wait_until(nextStep);
			nextStep = delayed_by_us(nextStep, PERIOD_US);
		}

		// End game wait for reset
		while (controller->step() != Controller::RESET)
		{
#ifdef PIMORONI
			led.set_rgb(50, 0, 0);
			sleep_ms(200);
			led.set_rgb(0, 0, 50);
#endif
			sleep_ms(200);
		}
	}
	return 0;
}
