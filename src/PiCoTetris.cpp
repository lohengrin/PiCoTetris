#include <stdio.h>

// PICO SDK
#include "pico/stdlib.h"

#include "Game.h"
#include "Display.h"

//! SSD1306 Driver
#include "DisplayDriverSSD1306.h"

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

	DisplayDriverSSD1306 disp_driver;
	Display disp(&disp_driver, Display::PORTRAIT);

	absolute_time_t  nextStep = delayed_by_us(get_absolute_time(),PERIOD_US);

	int i = 0;
	int inc = 1;

	while (true)
	{
		// Get joystick event


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
