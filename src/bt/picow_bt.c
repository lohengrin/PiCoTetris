/**
 * Copyright (c) 2022 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */
#include "picow_bt.h"

#include "btstack_audio.h"
#include "btstack_event.h"
#include "hal_led.h"
#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"

// Start the btstack example
int btstack_main(int argc, const char * argv[]);

int picow_bt_init(void) {
    // initialize CYW43 driver architecture (will enable BT if/because CYW43_ENABLE_BLUETOOTH == 1)
    if (cyw43_arch_init()) {
        printf("failed to initialise cyw43_arch\n");
        return -1;
    }

    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, 0);

    // inform about BTstack state
//    hci_event_callback_registration.callback = &packet_handler;
//    hci_add_event_handler(&hci_event_callback_registration);

    return 0;
}

void picow_bt_main(void) {

    btstack_main(0, NULL);
}
