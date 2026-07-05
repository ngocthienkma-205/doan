 Color Recognition System using STM32

This project is an embedded system application that detects colors using the TCS34725 RGB Color Sensor and displays the real-time color data on an I2C LCD screen, powered by an **STM32 microcontroller**.

 Hardware Components
    -Microcontroller: STM32 Series (e.g., STM32F103C8T6 / STM32F4)
    -Sensor: TCS34725 Color Sensor (RGB + Clear light sensing)
    -Display:LCD 16x2 / 20x4 with an I2C Adapter Module
    -Debugger/Programmer: ST-Link V2
    -Others: Breadboard, Jumper wires

Software & Tools
    -STM32CubeMX: Used for hardware initialization, pinout configuration, and clock setup (Configuration saved in `doan1.ioc`).
    -Keil uVision 5: Used as the main IDE for writing, compiling, and debugging C code (Project file: `doan1.uvprojx`).

 How to Build and Run
    -Clone or Download** this repository to your local machine.
    -Open the project by double-clicking `doan1.uvprojx` inside **Keil uVision 5**.
    -Connect your STM32 board to your computer using the **ST-Link V2**.
    -Press `F7` (or click **Build**) to compile the source code.
    -Press `F8` (or click **Download**) to flash the firmware onto the microcontroller.
    -Reset the board and see the sensor data displayed on the LCD screen.

File Structure
    - `i2c_lcd.c` / `i2c_lcd.h`: Driver library for controlling the LCD via I2C interface.
    - `TCS34725.c` / `TCS34725.h`: Driver library for reading RGB data from the color sensor.
    - `doan1.ioc`: STM32CubeMX configuration file.
