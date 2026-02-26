# I_move - Intelligent Eye Gesture Powered Wheelchair

## Overview
**I_move** is an intelligent wheelchair controlled by **eye gestures**.  
It combines Arduino-based hardware control with Python/Colab signal processing for gesture detection.

---

## Features
- Eye gesture detection for intuitive wheelchair control
- Motor control using Arduino
- Signal filtering and processing using Colab
- Modular, easy to extend, and educational

---

## Hardware Requirements
- Arduino Uno or compatible board
- DC motors + motor driver module
- Eye gesture sensors (IR or camera-based)
- Power supply

---

## Software Requirements
- Arduino IDE
- Python 3 (for offline tests)
- Google Colab (for signal processing notebook)

---

## Setup Instructions
1. Open `Arduino/motor.ino` and `Arduino/sensor.ino` in Arduino IDE.
2. Upload both sketches to the Arduino board.
3. Open `Colab/signal_processing.ipynb` in Google Colab.
4. Connect sensors and calibrate gestures using the notebook.
5. Test and control wheelchair with eye gestures.

---

## License
This project is for hackathon purposes. Free to use and modify for educational purposes.
