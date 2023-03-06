PiCoTetris
-----------

# Introduction

Rasberry pico (w) based Tetris like game (WIP)
First version is using a SSD106 Oled display connected to pins 24&25 (I2C1)
Display is working and only falling test piece.

# Compilation
Needs:
- [pico-sdk](https://github.com/raspberrypi/pico-sdk)
- [pimoroni-pico](https://github.com/pimoroni/pimoroni-pico)

```
$ mkdir build
$ cd build
$ cmake -DPICO_BOARD=pico_w ..
$ make
```

# Installation
- Copy uf2 file to the pico or use picotool: 
```
sudo picotool load -f -x PiCoTetris.uf2 
```

