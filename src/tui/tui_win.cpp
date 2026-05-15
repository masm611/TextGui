/*
Copyright 2026 masm611 <2838105183@qq.com>

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include "tui.hpp"

#include "windows.h"

#include <iostream>

static HANDLE hStdin;
static HANDLE hStdout;
static DWORD originalStdinConsoleMode;
static DWORD originalStdoutConsoleMode;
static bool modesSaved = false;

void Tui::init() {
    hStdin = GetStdHandle(STD_INPUT_HANDLE);
    if (hStdin == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("无法获取标准输入句柄");
    }

    if (!GetConsoleMode(hStdin, &originalStdinConsoleMode)) {
        throw std::runtime_error("无法获取原始控制台输入模式");
    }

    DWORD stdinConsoleMode = 0;

    stdinConsoleMode |= ENABLE_EXTENDED_FLAGS;
    stdinConsoleMode |= ENABLE_MOUSE_INPUT;

    if (!SetConsoleMode(hStdin, stdinConsoleMode)) {
        throw std::runtime_error("无法设置控制台输入模式");
        throw std::runtime_error("无法设置控制台模式");
    }

    hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout == INVALID_HANDLE_VALUE) {
        throw std::runtime_error("无法获取标准输出句柄");
    }

    // 保存原始输出模式
    if (!GetConsoleMode(hStdout, &originalStdoutConsoleMode)) {
        throw std::runtime_error("无法获取原始控制台输出模式");
    }

    // 启用输出VT
    DWORD consoleMode;
    if (!GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &consoleMode)) {
        throw std::runtime_error("无法获取控制台输出模式");
    }
    consoleMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    consoleMode |= ENABLE_PROCESSED_OUTPUT;
    consoleMode |= ENABLE_WRAP_AT_EOL_OUTPUT;
    if (!SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), consoleMode)) {
        std::cout << GetLastError() << std::endl;
        throw std::runtime_error("无法设置控制台输出模式");
    }

    modesSaved = true;

    // 设置编码页
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    // 测试一些字符的实际打印宽度
    radioBtnPrintWidth = testStrRealPrintWidth("●");
    arrowBtnPrintWidth = testStrRealPrintWidth("↑");

    // 隐藏光标
    CONSOLE_CURSOR_INFO cursorInfo = {1, FALSE};
    SetConsoleCursorInfo(hStdout, &cursorInfo);
}

void Tui::deinit() {
    if (!modesSaved)
        return;

    clearScreenBuffer(true);

    // 恢复光标可见性
    CONSOLE_CURSOR_INFO cursorInfo = {1, TRUE};
    SetConsoleCursorInfo(hStdout, &cursorInfo);

    // 恢复原始控制台模式
    SetConsoleMode(hStdin, originalStdinConsoleMode);
    SetConsoleMode(hStdout, originalStdoutConsoleMode);

    modesSaved = false;
}

void Tui::terminalUpdate() {
    static HWND hWndConsole = GetConsoleWindow();
    static int resizeFlushCnt = 0;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hStdout, &csbi)) {
        throw std::runtime_error("无法获取控制台屏幕缓冲区信息");
    }
    int currentWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int currentHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

    if (currentWidth != screenWidth || currentHeight != screenHeight) {
        if (currentWidth < screenWidth || currentHeight < screenHeight)
            adjustRootWidgetCenterPosOutOfScreen(Size(screenWidth, screenHeight), Size(currentWidth, currentHeight));

        resizeFlushCnt = 5;
    }

    if (csbi.srWindow.Left != 0 || csbi.srWindow.Top != 0) {
        resizeFlushCnt = 5;
    }

    CONSOLE_FONT_INFOEX fontInfo;
    fontInfo.cbSize = sizeof(CONSOLE_FONT_INFOEX);
    GetCurrentConsoleFontEx(hStdout, FALSE, &fontInfo);

    static int fontWidth = 0;
    static int fontHeight = 0;
    if (fontInfo.dwFontSize.X != fontWidth || fontInfo.dwFontSize.Y != fontHeight) {
        fontWidth = fontInfo.dwFontSize.X;
        fontHeight = fontInfo.dwFontSize.Y;

        if (IsZoomed(hWndConsole)) {
            ShowWindow(hWndConsole, SW_RESTORE);
            ShowWindow(hWndConsole, SW_MAXIMIZE);
            // std::this_thread::sleep_for(std::chrono::milliseconds(8));

            for (int i = 0; i < 18; ++i)
                ShowWindow(hWndConsole, SW_MAXIMIZE);
        }

        resizeFlushCnt = 5;
    }

    if (resizeFlushCnt > 0) {
        clearScreenBuffer(true);
        --resizeFlushCnt;
    }

    screenWidth = currentWidth;
    screenHeight = currentHeight;
}

void Tui::writeToConsole(const std::string& str) {
    WriteConsoleA(hStdout, str.c_str(), static_cast<DWORD>(str.length()), NULL, NULL);
}

void Tui::clearScreenBuffer(bool flush) {
    if (flush)
        WriteConsoleA(hStdout, "\x1b[0m\x1b[2J\x1b[3J", 12, NULL, NULL);
    WriteConsoleA(hStdout, "\x1b[H", 3, NULL, NULL);
}

std::tuple<int, int> Tui::getScreenBufferSize() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hStdout, &csbi)) {
        throw std::runtime_error("无法获取控制台屏幕缓冲区信息");
    }
    SHORT width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    SHORT height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return {width, height};
}

void Tui::eventReaderWorker() {
    while (!shouldQuit) {
        INPUT_RECORD ir[128];
        DWORD eventsRead;
        ReadConsoleInput(hStdin, ir, 128, &eventsRead);

        if (eventsRead > 0) {
            for (DWORD i = 0; i < eventsRead; ++i) {
                auto& event = ir[i];
                InputEvent inputEvent;

                if (event.EventType == MOUSE_EVENT) {
                    inputEvent.type = InputEvent::Type::Mouse;

                    inputEvent.mouseEvent.x = event.Event.MouseEvent.dwMousePosition.X;
                    inputEvent.mouseEvent.y = event.Event.MouseEvent.dwMousePosition.Y;
                    inputEvent.mouseEvent.leftButtonPressed = (event.Event.MouseEvent.dwButtonState & FROM_LEFT_1ST_BUTTON_PRESSED) != 0;
                    inputEvent.mouseEvent.rightButtonPressed = (event.Event.MouseEvent.dwButtonState & RIGHTMOST_BUTTON_PRESSED) != 0;
                    inputEvent.mouseEvent.eventType = MouseEventType::All;

                    if (event.Event.MouseEvent.dwEventFlags & MOUSE_WHEELED) {
                        if ((short)HIWORD(event.Event.MouseEvent.dwButtonState) > 0)
                            inputEvent.mouseEvent.eventType = MouseEventType::WheelUp;
                        else
                            inputEvent.mouseEvent.eventType = MouseEventType::WheelDown;
                    }
                } else if (event.EventType == KEY_EVENT) {
                    inputEvent.type = InputEvent::Type::Keyboard;

                    inputEvent.keyboardEvent.ctrlPressed = (event.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED)) != 0;
                    inputEvent.keyboardEvent.altPressed = (event.Event.KeyEvent.dwControlKeyState & (LEFT_ALT_PRESSED | RIGHT_ALT_PRESSED)) != 0;
                    inputEvent.keyboardEvent.shiftPressed = (event.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED) != 0;
                    inputEvent.keyboardEvent.keyChar = event.Event.KeyEvent.uChar.AsciiChar;
                    inputEvent.keyboardEvent.virtualKeyCode = event.Event.KeyEvent.wVirtualKeyCode;
                    inputEvent.keyboardEvent.eventType = event.Event.KeyEvent.bKeyDown ? KeyboardEventType::KeyDown : KeyboardEventType::KeyUp;
                } else {
                    continue;  // 其他事件类型暂不处理
                }

                {
                    std::lock_guard<std::mutex> lock(inputEventQueueMutex);
                    inputEventQueue.push(inputEvent);
                }
            }
        }
    }
}

int Tui::testStrRealPrintWidth(const std::string& str) {
    clearScreenBuffer(true);
    WriteConsoleA(hStdout, str.c_str(), static_cast<DWORD>(str.length()), NULL, NULL);

    // 获取光标位置
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        throw std::runtime_error("无法获取控制台屏幕缓冲区信息");
    }

    return csbi.dwCursorPosition.X;
}
