#include <stdio.h>

// PICO SDK
#include "pico/stdlib.h"

#include "Game.h"
#include "Display.h"
#include "Controller.h"

//! STDIN Controller
#include "ControllerStdin.h"

#ifdef SSD1306
	//! SSD1306 Driver
	#include "ssd1306/DisplayDriverSSD1306.h"
#endif

#ifdef PIMORONI
	//! PIMORONI Driver
	#include "pimoroni/DisplayDriverPimoroni.h"
	#include "pimoroni/ControllerPimoroni.h"
#endif

#ifdef RASPBERRYPI_PICO_W
#include "pico/cyw43_arch.h"
#endif

#include <memory>
#include <string.h>
#include <string>
#include <deque>
#include <numeric>

#define PERIOD_US 10000  // 100 Hz

int main()
{
	stdio_init_all();

#ifdef RASPBERRYPI_PICO_W
	// Init Wifi if using PICO_W (not used yet)
	if (cyw43_arch_init())
	{
		printf("WiFi init failed");
		return -1;
	}
#endif

	Game game(10,22);

// Display selection at compilation time
#ifdef PIMORONI
	std::unique_ptr<Controller> controller(new ControllerPimoroni);
	std::unique_ptr<DisplayDriver> disp_driver(new DisplayDriverPimoroni);
#endif
#ifdef SSD1306
	std::unique_ptr<Controller> controller(new ControllerStdin);
	std::unique_ptr<DisplayDriver> disp_driver(new DisplayDriverSSD1306);
#endif

	Display disp(disp_driver.get(), Display::PORTRAIT, game);

	absolute_time_t  nextStep = delayed_by_us(get_absolute_time(),PERIOD_US);

	int i = 0;
	int inc = 1;

	while (true)
	{
		// Get joystick event
		Controller::Command cmd = controller->step();

		game.setCommand(cmd);

		// Step Game
		game.step();

		// Display Game
		disp.draw(game);

		// Wait next step according to PERIOD_US
		busy_wait_until(nextStep);
		nextStep = delayed_by_us(nextStep,PERIOD_US);
	}
	return 0;
}
