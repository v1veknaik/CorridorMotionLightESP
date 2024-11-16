# Automated Corridor Lighting System using ESP32

## Description
A smart, motion-sensitive lighting control system using ESP32, IR/PIR sensors, and relays. It features a web interface for toggling between automatic and manual lighting modes, making it ideal for home and industrial applications.

## Features
- Motion detection using 3 IR/PIR sensors.
- Automatic and manual lighting modes via web interface.
- Remote control through Wi-Fi connectivity.
- Two relays for controlling lights and one GPIO for direct control.

## Components
- ESP32 Microcontroller
- IR/PIR Sensors (3)
- Relays (2 or 3)
- AC/DC LED/Light Bulbs (3)

## Note
- The number of LEDs/Bulbs is the user's discretion and can be implemented by using equivalent number of Relay(s) and making minimal changes to the code. Also one GPIO pin from the ESP32 was used in this project to allow the possibility of using LEDs w/o Relays incase of use in any Project with limited resources.

## Getting Started
1. Clone the repository:
   ```bash
   https://github.com/v1veknaik/CorridorMotionLightESP
   
