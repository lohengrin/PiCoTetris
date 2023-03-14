cmake_minimum_required(VERSION 3.13)

#Defualt to pico_w
if(NOT DEFINED PICO_BOARD)
	SET(PICO_BOARD pico_w)
endif()

option(PIMORONI_DISPLAY "Support for Pimoroni display pack (1,14\")" OFF)
option(SSD1306_DISPLAY "Support for ssd1306 display" OFF)

IF(NOT(${PIMORONI_DISPLAY} OR ${SSD1306_DISPLAY}))
    message( FATAL_ERROR "No Display selected, please use PIMORONI_DISPLAY or SSD1306_DISPLAY." )
ENDIF()


# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# Optionnal PIMORONI SDK for display
if(${PIMORONI_DISPLAY})
    include(pimoroni_pico_import.cmake)
endif()


project(PiCoTetris C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if(${PIMORONI_DISPLAY})
 include(common/pimoroni_i2c)
 include(common/pimoroni_bus)
 include(libraries/pico_display/pico_display)
 include(libraries/pico_graphics/pico_graphics)
 #include(libraries/bitmap_fonts/bitmap_fonts)
 #include(libraries/hershey_fonts/hershey_fonts)
 include(drivers/st7789/st7789)
 include(drivers/rgbled/rgbled)
 include(drivers/button/button)

 set(PIMORONI_SRC
  src/pimoroni/DisplayDriverPimoroni.h
  src/pimoroni/DisplayDriverPimoroni.cpp
  src/pimoroni/ControllerPimoroni.h
  src/pimoroni/ControllerPimoroni.cpp
 )
endif()

if (${SSD1306_DISPLAY})
 set(SSD1306_SRC
    # SSD1306 Display implementation
    src/ssd1306/DisplayDriverSSD1306.cpp
    src/ssd1306/DisplayDriverSSD1306.h
    src/ssd1306/ssd1306.cpp
    src/ssd1306/ssd1306.h
    src/ssd1306/font.h
 )
endif()

# rest of your project
add_executable(PiCoTetris
src/PiCoTetris.cpp
src/Game.cpp
src/Game.h
src/Display.cpp
src/Display.h
src/DisplayDriver.h
src/Controller.h
src/ControllerStdin.h
src/ControllerStdin.cpp
src/Piece.cpp
src/Piece.h
src/Colors.cpp
src/Colors.h

${PIMORONI_SRC}
${SSD1306_SRC}
)

target_include_directories(PiCoTetris PRIVATE src)

# Add pico_stdlib library which aggregates commonly used features
target_link_libraries(PiCoTetris 
    pico_stdlib
    hardware_i2c
)

if(${PIMORONI_DISPLAY})
    target_link_libraries(PiCoTetris 
        hardware_spi
        hardware_pwm
        hardware_dma
        pico_display
        st7789
        rgbled
        button
    )
target_compile_definitions(PiCoTetris PRIVATE PIMORONI)
endif()

if (${SSD1306_DISPLAY})
    target_link_libraries(PiCoTetris 
        hardware_i2c
    )
    target_compile_definitions(PiCoTetris PRIVATE SSD1306)
endif()

IF (${PICO_BOARD} STREQUAL "pico_w")
target_link_libraries(PiCoTetris 
    pico_cyw43_arch_none
)
ENDIF()

pico_enable_stdio_usb(PiCoTetris 1)
pico_enable_stdio_uart(PiCoTetris 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(PiCoTetris)


