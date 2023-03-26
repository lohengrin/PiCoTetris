// SPDX-License-Identifier: Apache-2.0
// Copyright 2022 Ricardo Quesada
// http://retro.moe/unijoysticle2

#include "PicoConsole.h"
#include "uni_log.h"

using namespace Bluepad32;

void Console::print(const std::string& str) {
    uni_log("%s", str.c_str());
}

void Console::print(const char* str) {
    uni_log("%s", str);
}

void Console::println(const std::string& str) {
    uni_log("%s\n", str.c_str());
}

void Console::println(const char* str) {
    uni_log("%s\n", str);
}

void Console::printf(const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    uni_logv(fmt, args);
    va_end(args);
}

class Console Console;