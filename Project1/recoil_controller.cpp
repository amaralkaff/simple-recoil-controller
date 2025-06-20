﻿#include <windows.h>
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#pragma comment(lib, "setupapi.lib")

bool isInsertOn() {
    return (GetKeyState(VK_INSERT) & 0x0001);
}

bool isLeftPressed() {
    return (GetAsyncKeyState(VK_LBUTTON) & 0x8000);
}

bool isRightPressed() {
    return (GetAsyncKeyState(VK_RBUTTON) & 0x8000);
}

std::string find_arduino_micro_port() {
    HDEVINFO device_info_set = SetupDiGetClassDevs(&GUID_DEVCLASS_PORTS, nullptr, nullptr, DIGCF_PRESENT);
    if (device_info_set == INVALID_HANDLE_VALUE)
        return "";

    SP_DEVINFO_DATA device_info_data;
    device_info_data.cbSize = sizeof(SP_DEVINFO_DATA);

    char buffer[256];
    for (DWORD i = 0; SetupDiEnumDeviceInfo(device_info_set, i, &device_info_data); ++i) {
        if (SetupDiGetDeviceRegistryPropertyA(
            device_info_set, &device_info_data, SPDRP_FRIENDLYNAME,
            nullptr, (PBYTE)buffer, sizeof(buffer), nullptr)) {
            std::string name = buffer;
            if (name.find("Arduino Micro") != std::string::npos ||
                name.find("Arduino Leonardo") != std::string::npos ||
                name.find("USB Serial Device") != std::string::npos) {
                size_t begin = name.find("COM");
                size_t end = name.find(")", begin);
                if (begin != std::string::npos && end != std::string::npos) {
                    std::string port = name.substr(begin, end - begin);
                    SetupDiDestroyDeviceInfoList(device_info_set);
                    return "\\\\.\\" + port;
                }
            }
        }
    }

    SetupDiDestroyDeviceInfoList(device_info_set);
    return "";
}

int main() {
    std::string port = find_arduino_micro_port();
    if (port.empty()) {
        std::cerr << "❌ Could not auto-detect Arduino Micro/Leonardo!\n";
        return 1;
    }

    std::cout << "✅ Found Arduino on " << port << "\n";

    HANDLE hSerial = CreateFileA(
        port.c_str(),
        GENERIC_READ | GENERIC_WRITE,
        0,
        nullptr,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "❌ Failed to open serial port!\n";
        return 1;
    }

    // Serial config
    DCB dcb = { 0 };
    dcb.DCBlength = sizeof(DCB);
    GetCommState(hSerial, &dcb);
    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.StopBits = ONESTOPBIT;
    dcb.Parity = NOPARITY;
    SetCommState(hSerial, &dcb);

    std::cout << "🎮 RECOIL CONTROLLER READY\n";
    std::cout << "═══════════════════════════\n";
    std::cout << "Controls: Insert + Right+Left Click\n";
    std::cout << "Status: Waiting...\n\n";

    bool recoilActive = false;
    bool insertPreviousState = false;

    while (true) {
        bool insertCurrentState = isInsertOn();

        // Show Insert key status changes
        if (insertCurrentState != insertPreviousState) {
            if (insertCurrentState) {
                std::cout << "🟢 INSERT: ON  - Ready to activate\n";
            }
            else {
                std::cout << "🔴 INSERT: OFF - Disabled\n";
            }
            insertPreviousState = insertCurrentState;
        }
        if (insertCurrentState) {
            if (isRightPressed() && isLeftPressed() && !recoilActive) {
                char startCmd = 's';
                DWORD bytesWritten;
                WriteFile(hSerial, &startCmd, 1, &bytesWritten, nullptr);
                std::cout << "⚡ RECOIL: ACTIVE - Compensating...\n";
                recoilActive = true;
            }

            if (!isLeftPressed() && recoilActive) {
                char stopCmd = 'x';
                DWORD bytesWritten;
                WriteFile(hSerial, &stopCmd, 1, &bytesWritten, nullptr);
                std::cout << "⏸️  RECOIL: PAUSED - Waiting for input...\n";
                recoilActive = false;
            }
        }
        else if (recoilActive) {
            char stopCmd = 'x';
            DWORD bytesWritten;
            WriteFile(hSerial, &stopCmd, 1, &bytesWritten, nullptr);
            std::cout << "🛑 RECOIL: STOPPED - Insert key disabled\n";
            recoilActive = false;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    CloseHandle(hSerial);
    return 0;
}
