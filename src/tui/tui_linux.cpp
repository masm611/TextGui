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

#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <clocale>
#include <cstdlib>
#include <stdexcept>

static struct termios originalTermios;
static bool termiosSaved = false;

void Tui::init() {
    if (tcgetattr(STDIN_FILENO, &originalTermios) == -1)
        throw std::runtime_error("无法获取终端属性");

    struct termios raw = originalTermios;
    raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
    raw.c_oflag &= ~(OPOST);
    raw.c_cflag |= (CS8);
    raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
    raw.c_cc[VMIN] = 0;
    raw.c_cc[VTIME] = 1;

    if (tcsetattr(STDIN_FILENO, TCSANOW, &raw) == -1)
        throw std::runtime_error("无法设置终端为raw模式");

    termiosSaved = true;

    write(STDOUT_FILENO, "\x1b[?1000h", 8);
    write(STDOUT_FILENO, "\x1b[?1003h", 8);
    write(STDOUT_FILENO, "\x1b[?1006h", 8);

    radioBtnPrintWidth = testStrRealPrintWidth("●");
    arrowBtnPrintWidth = testStrRealPrintWidth("↑");

    write(STDOUT_FILENO, "\x1b[?25l", 7);
}

void Tui::deinit() {
    if (!termiosSaved)
        return;

    // 重置所有颜色和样式属性
    // write(STDOUT_FILENO, "\x1b[0m", 4);
    clearScreenBuffer(true);

    // 恢复光标可见性
    write(STDOUT_FILENO, "\x1b[?25h", 7);

    // 禁用鼠标追踪
    write(STDOUT_FILENO, "\x1b[?1006l", 8);
    write(STDOUT_FILENO, "\x1b[?1003l", 8);
    write(STDOUT_FILENO, "\x1b[?1000l", 8);

    // 恢复原始终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &originalTermios);

    termiosSaved = false;
}

void Tui::terminalUpdate() {
    static int resizeFlushCnt = 0;

    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        return;

    int currentWidth = ws.ws_col;
    int currentHeight = ws.ws_row;

    if (currentWidth != screenWidth || currentHeight != screenHeight) {
        if (currentWidth < screenWidth || currentHeight < screenHeight)
            adjustRootWidgetCenterPosOutOfScreen(Size(screenWidth, screenHeight), Size(currentWidth, currentHeight));

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
    write(STDOUT_FILENO, str.c_str(), str.length());
}

void Tui::clearScreenBuffer(bool flush) {
    if (flush)
        write(STDOUT_FILENO, "\x1b[0m\x1b[2J\x1b[3J", 12);
    write(STDOUT_FILENO, "\x1b[H", 3);
}

std::tuple<int, int> Tui::getScreenBufferSize() {
    struct winsize ws;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1)
        throw std::runtime_error("无法获取终端窗口大小");
    return {ws.ws_col, ws.ws_row};
}

static void parseCSIKeyboard(const std::string& seq, std::queue<InputEvent>& queue, std::mutex& mtx) {
    InputEvent ie;
    ie.type = InputEvent::Type::Keyboard;
    ie.keyboardEvent.ctrlPressed = false;
    ie.keyboardEvent.altPressed = false;
    ie.keyboardEvent.shiftPressed = false;
    ie.keyboardEvent.keyChar = 0;
    ie.keyboardEvent.eventType = KeyboardEventType::KeyDown;

    char final = seq.back();
    int vk = 0;

    switch (final) {
        case 'A':
            vk = 38;
            break;  // 上箭头
        case 'B':
            vk = 40;
            break;  // 下箭头
        case 'C':
            vk = 39;
            break;  // 右箭头
        case 'D':
            vk = 37;
            break;  // 左箭头
        case 'H':
            vk = 36;
            break;  // Home
        case 'F':
            vk = 35;
            break;  // End
        default:
            break;
    }

    if (final == '~') {
        const char* params = seq.c_str() + 2;
        int param = atoi(params);
        switch (param) {
            case 1:
                vk = 36;
                break;  // Home
            case 2:
                vk = 45;
                break;  // Insert
            case 3:
                vk = 46;
                break;  // Delete
            case 4:
                vk = 35;
                break;  // End
            case 5:
                vk = 33;
                break;  // PgUp
            case 6:
                vk = 34;
                break;  // PgDn
            default:
                break;
        }
    }

    ie.keyboardEvent.virtualKeyCode = vk;
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(ie);
    }
}

static void parseSS3Keyboard(const std::string& seq, std::queue<InputEvent>& queue, std::mutex& mtx) {
    InputEvent ie;
    ie.type = InputEvent::Type::Keyboard;
    ie.keyboardEvent.ctrlPressed = false;
    ie.keyboardEvent.altPressed = false;
    ie.keyboardEvent.shiftPressed = false;
    ie.keyboardEvent.keyChar = 0;
    ie.keyboardEvent.eventType = KeyboardEventType::KeyDown;

    int vk = 0;
    switch (seq[2]) {
        case 'P':
            vk = 112;
            break;  // F1
        case 'Q':
            vk = 113;
            break;  // F2
        case 'R':
            vk = 114;
            break;  // F3
        case 'S':
            vk = 115;
            break;  // F4
        default:
            break;
    }

    ie.keyboardEvent.virtualKeyCode = vk;
    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(ie);
    }
}

static void parseSGRMouse(const std::string& seq, std::queue<InputEvent>& queue, std::mutex& mtx,
                          bool& leftDown, bool& rightDown) {
    InputEvent ie;
    ie.type = InputEvent::Type::Mouse;
    ie.mouseEvent.leftButtonPressed = false;
    ie.mouseEvent.rightButtonPressed = false;
    ie.mouseEvent.eventType = MouseEventType::All;
    ie.mouseEvent.x = 0;
    ie.mouseEvent.y = 0;

    const char* p = seq.c_str() + 3;  // 跳过 ESC [ <
    int button = atoi(p);
    while (*p && *p != ';') p++;
    if (*p != ';') return;
    p++;
    int mx = atoi(p);
    while (*p && *p != ';') p++;
    if (*p != ';') return;
    p++;
    int my = atoi(p);

    bool isRelease = (seq.back() == 'm');

    switch (button) {
        case 0:
            leftDown = !isRelease;
            break;
        case 2:
            rightDown = !isRelease;
            break;
        case 64:
            ie.mouseEvent.eventType = MouseEventType::WheelUp;
            break;
        case 65:
            ie.mouseEvent.eventType = MouseEventType::WheelDown;
            break;
        default:
            break;
    }

    ie.mouseEvent.x = mx - 1;
    ie.mouseEvent.y = my - 1;
    ie.mouseEvent.leftButtonPressed = leftDown;
    ie.mouseEvent.rightButtonPressed = rightDown;

    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(ie);
    }
}

static void sendAltKey(const std::string& seq, std::queue<InputEvent>& queue, std::mutex& mtx) {
    InputEvent ie;
    ie.type = InputEvent::Type::Keyboard;
    ie.keyboardEvent.ctrlPressed = false;
    ie.keyboardEvent.altPressed = true;
    ie.keyboardEvent.shiftPressed = false;
    ie.keyboardEvent.keyChar = seq[1];
    ie.keyboardEvent.virtualKeyCode = seq[1];
    ie.keyboardEvent.eventType = KeyboardEventType::KeyDown;

    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(ie);
    }
}

static void processRegularByte(unsigned char c, std::queue<InputEvent>& queue, std::mutex& mtx) {
    InputEvent ie;
    ie.type = InputEvent::Type::Keyboard;
    ie.keyboardEvent.eventType = KeyboardEventType::KeyDown;
    ie.keyboardEvent.altPressed = false;
    ie.keyboardEvent.shiftPressed = false;

    switch (c) {
        case 0x0D:
            ie.keyboardEvent.ctrlPressed = false;
            ie.keyboardEvent.keyChar = '\n';
            ie.keyboardEvent.virtualKeyCode = 13;
            break;
        case 0x7F:
            ie.keyboardEvent.ctrlPressed = false;
            ie.keyboardEvent.keyChar = '\b';
            ie.keyboardEvent.virtualKeyCode = 8;
            break;
        case 0x09:
            ie.keyboardEvent.ctrlPressed = false;
            ie.keyboardEvent.keyChar = '\t';
            ie.keyboardEvent.virtualKeyCode = 9;
            break;
        case 0x1B:
            ie.keyboardEvent.ctrlPressed = false;
            ie.keyboardEvent.keyChar = 0;
            ie.keyboardEvent.virtualKeyCode = 27;
            break;
        case '\b':
            ie.keyboardEvent.ctrlPressed = false;
            ie.keyboardEvent.keyChar = '\b';
            ie.keyboardEvent.virtualKeyCode = 8;
            break;
        default:
            if (c > 0 && c < 0x20) {
                ie.keyboardEvent.ctrlPressed = true;
                ie.keyboardEvent.keyChar = c + 0x60;
                ie.keyboardEvent.virtualKeyCode = (c + 0x40);
            } else {
                ie.keyboardEvent.ctrlPressed = false;
                ie.keyboardEvent.keyChar = c;
                ie.keyboardEvent.virtualKeyCode = c;
            }
            break;
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        queue.push(ie);
    }
}

static bool isCSIComplete(const std::string& seq) {
    char last = seq.back();
    return last >= 0x40 && last <= 0x7E && last != '[';
}

void Tui::eventReaderWorker() {
    char buf[256];
    std::string escapeSeq;

    bool mouseLeftDown = false;
    bool mouseRightDown = false;

    while (!shouldQuit) {
        int n = read(STDIN_FILENO, buf, sizeof(buf));
        if (n <= 0)
            continue;

        for (int i = 0; i < n; ++i) {
            unsigned char c = buf[i];

            if (!escapeSeq.empty()) {
                escapeSeq += (char)c;

                if (escapeSeq.size() >= 2 && escapeSeq[1] == '[') {
                    if (isCSIComplete(escapeSeq)) {
                        if (escapeSeq.size() >= 3 && escapeSeq[2] == '<')
                            parseSGRMouse(escapeSeq, inputEventQueue, inputEventQueueMutex, mouseLeftDown, mouseRightDown);
                        else
                            parseCSIKeyboard(escapeSeq, inputEventQueue, inputEventQueueMutex);
                        escapeSeq.clear();
                    }
                } else if (escapeSeq.size() >= 2 && escapeSeq[1] == 'O') {
                    if (escapeSeq.size() >= 3) {
                        parseSS3Keyboard(escapeSeq, inputEventQueue, inputEventQueueMutex);
                        escapeSeq.clear();
                    }
                } else {
                    sendAltKey(escapeSeq, inputEventQueue, inputEventQueueMutex);
                    escapeSeq.clear();
                }
            } else if (c == 0x1B) {
                escapeSeq = "\x1B";
            } else {
                processRegularByte(c, inputEventQueue, inputEventQueueMutex);
            }
        }
    }
}

int Tui::testStrRealPrintWidth(const std::string& str) {
    clearScreenBuffer(true);
    write(STDOUT_FILENO, str.c_str(), str.length());

    write(STDOUT_FILENO, "\x1b[6n", 4);

    struct timeval tv = {0, 200000};
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);

    if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) <= 0)
        return 0;

    char response[32] = {};
    int n = read(STDIN_FILENO, response, sizeof(response) - 1);
    if (n <= 0)
        return 0;
    response[n] = '\0';

    const char* p = response;
    if (p[0] == 0x1B && p[1] == '[')
        p += 2;
    while (*p && *p >= '0' && *p <= '9')
        p++;
    if (*p == ';')
        p++;

    int col = atoi(p);
    return (col > 0) ? col - 1 : 0;
}
