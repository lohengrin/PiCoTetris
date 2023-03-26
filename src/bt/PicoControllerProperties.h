// Copyright 2021 - 2023, Ricardo Quesada, http://retro.moe
// SPDX-License-Identifier: Apache-2.0 or LGPL-2.1-or-later
#pragma once

#include "sdkconfig.h"
#include <uni_platform_pico.h>

namespace Bluepad32
{
    using ControllerProperties = pico_controller_properties_t;
    typedef ControllerProperties* ControllerPropertiesPtr;
}

