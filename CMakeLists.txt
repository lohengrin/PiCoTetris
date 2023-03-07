cmake_minimum_required(VERSION 3.13)

#Defualt to pico_w
if(NOT DEFINED PICO_BOARD)
	SET(PICO_BOARD pico_w)
endif()

# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

project(PiCoTetris C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

# rest of your project
add_executable(PiCoTetris
src/PiCoTetris.cpp
src/Game.cpp
src/Game.h
src/Display.cpp
src/Display.h
src/DisplayDriver.h
src/DisplayDriverSSD1306.cpp
src/DisplayDriverSSD1306.h
src/Controller.h
src/ControllerStdin.h
src/ControllerStdin.cpp
src/Piece.cpp
src/Piece.h
src/ssd1306.cpp
src/ssd1306.h
src/font.h
)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(PiCoTetris 
    pico_stdlib
    hardware_i2c
)

IF (${PICO_BOARD} STREQUAL "pico_w")
target_link_libraries(PiCoTetris 
    pico_cyw43_arch_none
)
ENDIF()

pico_enable_stdio_usb(PiCoTetris 1)
pico_enable_stdio_uart(PiCoTetris 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(PiCoTetris)


