cmake_minimum_required(VERSION 3.13)

#Defualt to pico_w
if(NOT DEFINED PICO_BOARD)
	SET(PICO_BOARD pico_w)
endif()


SET(DVI_DEFAULT_SERIAL_CONFIG pimoroni_demo_hdmi_cfg)

option(PIMORONI_DISPLAY "Support for Pimoroni display pack (1,14\")" OFF)
option(SSD1306_DISPLAY "Support for ssd1306 display" OFF)
option(DVI_DISPLAY "Support for DVI display" OFF)

IF(NOT(${PIMORONI_DISPLAY} OR ${SSD1306_DISPLAY} OR ${DVI_DISPLAY}))
    message( FATAL_ERROR "No Display selected, please use PIMORONI_DISPLAY, SSD1306_DISPLAY or DVI_DISPLAY." )
ENDIF()


# initialize the SDK based on PICO_SDK_PATH
# note: this must happen before project()
include(pico_sdk_import.cmake)

# Optionnal PIMORONI SDK for display
if(${PIMORONI_DISPLAY} OR ${DVI_DISPLAY})
    include(pimoroni_pico_import.cmake)
endif()


project(PiCoTetris C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# initialize the Raspberry Pi Pico SDK
pico_sdk_init()

if (${DVI_DISPLAY})
    include(libraries/pico_graphics/pico_graphics)

    set(DVI_SRC
        src/dvi/DisplayDriverDvi.h
        src/dvi/DisplayDriverDvi.cpp
        src/dvi/ControllerDvi.h
        src/dvi/ControllerDvi.cpp
  )
endif()

if(${PIMORONI_DISPLAY})
 include(common/pimoroni_i2c)
 include(common/pimoroni_bus)
 include(libraries/pico_display/pico_display)
 include(libraries/pico_graphics/pico_graphics)
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


if(${DVI_DISPLAY})
    add_subdirectory(libdvi)
    add_subdirectory(libsprite)
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
${DVI_SRC}
)

if(${DVI_DISPLAY})
    target_compile_options(PiCoTetris PRIVATE -Wall -Wno-narrowing)
    target_compile_definitions(PiCoTetris PRIVATE
    DVI_DEFAULT_SERIAL_CONFIG=${DVI_DEFAULT_SERIAL_CONFIG}
    SDCARD_PIN_SPI0_CS=22
    SDCARD_PIN_SPI0_SCK=5
    SDCARD_PIN_SPI0_MOSI=18
    SDCARD_PIN_SPI0_MISO=19
    SDCARD_PIO=pio1
    SDCARD_PIO_SM=0
    # compile time configuration of I2S
    PICO_AUDIO_I2S_MONO_INPUT=1
    #define for our example code
    USE_AUDIO_I2S=1
    PICO_AUDIO_I2S_DMA_IRQ=1
    PICO_AUDIO_I2S_PIO=1
    PICO_AUDIO_I2S_DATA=26
    PICO_AUDIO_I2S_BCLK=27
    )

    target_link_libraries(PiCoTetris
		pico_multicore
		pico_util
		libdvi
        libsprite
        pico_graphics
    )

    target_compile_definitions(PiCoTetris PRIVATE DVI)
endif()


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

pico_enable_stdio_usb(PiCoTetris 1)
pico_enable_stdio_uart(PiCoTetris 0)

# create map/bin/hex/uf2 file in addition to ELF.
pico_add_extra_outputs(PiCoTetris)


