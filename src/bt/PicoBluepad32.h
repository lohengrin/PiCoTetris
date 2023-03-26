// Copyright 2021 - 2021, Ricardo Quesada, http://retro.moe
// SPDX-License-Identifier: Apache 2.0 or LGPL-2.1-or-later

#pragma once

#include "sdkconfig.h"

#include <inttypes.h>
#include <uni_platform_pico.h>
#include <functional>

#include "PicoController.h"

// Pico friendly define.
#define BP32_MAX_CONTROLLERS CONFIG_BLUEPAD32_MAX_DEVICES
#define BP32_MAX_GAMEPADS BP32_MAX_CONTROLLERS

namespace Bluepad32
{
    typedef std::function<void(ControllerPtr controller)> ControllerCallback;
    using GamepadCallback = ControllerCallback;

    class PicoBluepad32
    {
        // This is used internally by SPI, and then copied into the Controller::State of
        // each controller
        int _prevConnectedControllers;

        // This is what the user receives
        Controller _controllers[BP32_MAX_CONTROLLERS];

        ControllerCallback _onConnect;
        ControllerCallback _onDisconnect;

    public:
        PicoBluepad32();
        /*
         * Get the firmware version
         * result: version as string with this format a.b.c
         */
        const char *firmwareVersion() const;
        void setDebug(uint8_t on);

        // Controller
        void update();

        // When a controller is paired to the ESP32, the ESP32 stores keys to enable reconnection.
        // If you want to "forget" (delete) the keys from ESP32, you should call this
        // function.
        void forgetBluetoothKeys();

        // Enable / Disable new Bluetooth connections.
        // When enabled, the device is put in Discovery mode, and new pairs are accepted.
        // When disabled, only devices that have paired before can connect.
        // Established connections are not affected.
        void enableNewBluetoothConnections(bool enabled);

        void setup(const ControllerCallback &onConnect, const ControllerCallback &onDisconnect);

    private:
        void checkProtocol();
    };

    extern PicoBluepad32 BP32;
};
