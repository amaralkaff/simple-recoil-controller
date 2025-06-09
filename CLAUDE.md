# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is an Arduino-based recoil control system for gaming. The project consists of two main components:

1. **Arduino firmware** (`arduino_recoil.ino`) - Runs on Arduino Micro, receives serial commands and performs mouse movements to counteract recoil
2. **Windows controller** (`recoil_controller.cpp`) - C++ application that monitors keyboard/mouse input and communicates with Arduino

## How to Use

### Quick Setup
1. Connect Arduino Micro to your PC
2. Upload `arduino_recoil.ino` to the Arduino using Arduino IDE
3. Compile and run `recoil_controller.exe`
4. Turn on Scroll Lock
5. Hold Right Mouse Button + Left Mouse Button to activate recoil control

### Step-by-Step Usage
1. **Hardware Setup**: Connect Arduino Micro via USB
2. **Upload Firmware**: Open Arduino IDE, load `arduino_recoil.ino`, select Arduino Micro board, upload
3. **Run Controller**: Execute `recoil_controller.exe` (will auto-detect Arduino port)
4. **Activate**: Enable Scroll Lock, then hold RMB + LMB to start recoil compensation
5. **Deactivate**: Release LMB or turn off Scroll Lock to stop

## Architecture

The system uses a two-part architecture:

- **Windows Controller**: Monitors for Scroll Lock + Right+Left mouse button combination, auto-detects Arduino Micro COM port, sends start/stop commands via serial
- **Arduino Firmware**: Listens for serial commands ('s' to start, 'x' to stop), executes predefined mouse movement pattern when active

Communication happens over 9600 baud serial connection between Windows PC and Arduino Micro.

## Development Commands

### Arduino Development
1. Open Arduino IDE
2. Load `arduino_recoil.ino`
3. Select Board: Arduino Micro
4. Select COM Port (check in Device Manager)
5. Upload the sketch (Ctrl + U)

### Windows Controller Development
- Compile: `g++ -o recoil_controller.exe recoil_controller.cpp -lsetupapi`
- Or use Visual Studio with setupapi.lib linked
- Requires Windows SDK for SetupAPI functions
- Auto-detects Arduino Micro COM port using Windows Device Manager APIs

### Important Notes
- Arduino must be recognized as "Arduino Micro" in Device Manager
- Do NOT have Arduino Serial Monitor open at the same time
- Movement pattern is adjustable in the sketch (delay and offsets)

## Key Implementation Details

- Mouse movement pattern: `Mouse.move(-10, 12)` followed by `Mouse.move(10, -10)` with 10ms delays
- Input detection uses Windows `GetKeyState()` and `GetAsyncKeyState()` APIs
- Arduino port detection searches for "Arduino Micro" in device friendly names
- Recoil stops when Left mouse button is released or Scroll Lock is turned off