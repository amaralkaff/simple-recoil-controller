# Arduino Recoil Controller

Simple Arduino-based recoil control system for gaming.

## What it does

- Automatically counteracts weapon recoil in games
- Uses Arduino Micro to simulate mouse movements
- Activates with Insert + Right Mouse + Left Mouse buttons

## Hardware Required

- Arduino Micro or Arduino Leonardo
- USB cable

## Quick Setup

1. **Upload Arduino code**:
   - Open Arduino IDE
   - Load `arduino_recoil.ino`
   - Select "Arduino Micro" or "Arduino Leonardo" board
   - Upload to your Arduino

2. **Build Windows controller**:
   
   **Option A - Command Line:**
   ```bash
   g++ -o recoil_controller.exe recoil_controller.cpp -lsetupapi
   ```
   
   **Option B - Visual Studio:**
   - Open `Project1.sln` or `RecoilController.sln`
   - Build > Build Solution (Ctrl+Shift+B)
   - Executable will be in `x64/Release/` or `x64/Debug/`

3. **Run**:
   - Connect Arduino Micro to PC
   - Run `recoil_controller.exe`
   - Enable Scroll Lock
   - Hold Right Mouse Button + Left Mouse Button to activate

## How to Use

1. Turn on Scroll Lock (indicator light should be on)
2. Hold Right Mouse Button
3. Press and hold Left Mouse Button - recoil control starts
4. Release Left Mouse Button - recoil control stops
