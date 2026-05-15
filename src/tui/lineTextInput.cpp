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

#include "lineTextInput.hpp"

#include "painter.hpp"
#include "utf8Utils.hpp"
#include "tui.hpp"

LineTextInput::LineTextInput(Widget* parent) : Widget(parent) {
}

void LineTextInput::draw() {
    Widget::draw();

    Painter painter(this);
    painter.fill(Rect(0, 0, geometry().width, geometry().height), Color(200, 200, 200));
    painter.drawText(Rect(0, 0, geometry().width, 1), std::span<const Utf8Char>(inputBuffer.data() + displayStartPosition, inputBuffer.size() - displayStartPosition), Color(0, 0, 0));

    Tui* tui = Tui::instance();
    if (tui->getFocusedWidget() == this) {
        auto now = std::chrono::steady_clock::now();
        auto timeSinceLastBlink = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastBlinkTime).count();
        if (timeSinceLastBlink >= 500) {
            cursorVisible = !cursorVisible;
            lastBlinkTime = now;
        }
    } else {
        cursorVisible = false;
        lastBlinkTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(9999);
    }

    if (cursorVisible) {
        // 反转光标位置的前背景色
        short screenCursorU8cCntX = cursorPosition - displayStartPosition;
        short screenCursorX = 0;
        for (short i = 0; i < screenCursorU8cCntX; ++i)
            screenCursorX += getPrintWidth(inputBuffer[i + displayStartPosition]);
        painter.reverseFgBgColor(screenCursorX, 0);
    }
}

std::string LineTextInput::getText() {
    return u8cArrayToString(inputBuffer);
}

void LineTextInput::setText(const std::string& str) {
    bool changed = false;
    auto newBuffer = getUtf8Chars(str);

    if (newBuffer.size() != inputBuffer.size()) {
        changed = true;
    } else
        for (int i = 0; i < inputBuffer.size() && i < newBuffer.size(); ++i) {
            if (inputBuffer[i] != newBuffer[i]) {
                changed = true;
                break;
            }
        }

    if (changed) {
        inputBuffer = newBuffer;
        ensureCursorVisible();

        for (auto& listener : textChangedEventListeners)
            std::get<0>(listener)(str);
    }
}

EventProcessResult LineTextInput::keyboardEvent(const KeyboardEvent& event) {
    Widget::keyboardEvent(event);

    if (event.eventType == KeyboardEventType::CharInput) {
        if (event.keyChar == '\b') {
            // 退格键
            if (cursorPosition > 0) {
                inputBuffer.erase(inputBuffer.begin() + cursorPosition - 1);
                --cursorPosition;
            }
        } else if (event.keyChar == '\n')
            return EventProcessResult::Finished;
        else {
            // 普通输入
            if (event.keyChar >= 0 && event.keyChar <= 127) {
                // ascii字符
                inputBuffer.insert(inputBuffer.begin() + cursorPosition, Utf8Char{(uint8_t)event.keyChar, 0, 0, 0});
                ++cursorPosition;
            } else {
                if ((event.keyChar & 0b11000000) == 0b10000000) {
                    // u8延续字符
                    if (cursorPosition == 0)
                        return EventProcessResult::Finished;
                    Utf8Char& lastChar = inputBuffer[cursorPosition - 1];
                    for (int i = 0; i < 4; ++i) {
                        if (lastChar[i] == 0) {
                            lastChar[i] = (uint8_t)event.keyChar;
                            break;
                        }
                    }
                } else {
                    // u8首字符
                    inputBuffer.insert(inputBuffer.begin() + cursorPosition, Utf8Char{(uint8_t)event.keyChar, 0, 0, 0});
                    ++cursorPosition;
                }
            }
        }

        cursorVisible = true;
        lastBlinkTime = std::chrono::steady_clock::now();

        for (auto& listener : textChangedEventListeners)
            std::get<0>(listener)(getText());

        ensureCursorVisible();
        return EventProcessResult::Finished;
    }

    if (event.eventType == KeyboardEventType::SpecialInput) {
        if (event.virtualKeyCode == SpecialVirtualKeyCode::LeftArrow) {
            if (cursorPosition > 0)
                --cursorPosition;
        } else if (event.virtualKeyCode == SpecialVirtualKeyCode::RightArrow) {
            if (cursorPosition < inputBuffer.size())
                ++cursorPosition;
        }

        cursorVisible = true;
        lastBlinkTime = std::chrono::steady_clock::now();

        ensureCursorVisible();
        return EventProcessResult::Finished;
    }

    return EventProcessResult::PassDown;
}

EventProcessResult LineTextInput::mouseEvent(const MouseEvent& event) {
    Widget::mouseEvent(event);

    if (event.eventType == MouseEventType::Click) {
        // 计算点击位置对应的字符索引
        cursorPosition = getU8cIndexByDisplayPos(event.x);

        cursorVisible = true;
        lastBlinkTime = std::chrono::steady_clock::now();

        ensureCursorVisible();
        return EventProcessResult::Finished;
    }

    return EventProcessResult::PassDown;
}

int LineTextInput::addEventListener(const std::string& eventName, std::any callback) {
    if (eventName == "textChanged") {
        if (callback.type() != typeid(std::function<void(const std::string&)>))
            throw std::invalid_argument("回调函数类型错误，应该为 std::function<void(const std::string&)>");

        int newId = ((textChangedEventListeners.size() == 0) ? 1 : std::get<1>(*(textChangedEventListeners.end() - 1)) + 1);
        textChangedEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void(const std::string&)>>(callback), newId));
        return newId;
    } else {
        return Widget::addEventListener(eventName, callback);
    }
}

void LineTextInput::ensureCursorVisible() {
    if (cursorPosition > inputBuffer.size())
        cursorPosition = static_cast<int>(inputBuffer.size());

    int screenCursorU8cCntX = cursorPosition - displayStartPosition;

    int currentX = 0;
    for (int i = 0; i < screenCursorU8cCntX; ++i)
        currentX += getPrintWidth(inputBuffer[i + displayStartPosition]);

    if (screenCursorU8cCntX <= 0) {
        // 光标在左边界或者左边界外，调整显示起始位置
        displayStartPosition = (cursorPosition - 1 >= 0) ? cursorPosition - 1 : 0;
    } else if (currentX >= geometry().width) {
        // 光标在右边界外，调整显示起始位置
        displayStartPosition += currentX - geometry().width + 1;
    }
}

int LineTextInput::getU8cIndexByDisplayPos(int displayPosX) {
    int charIndex = displayStartPosition;
    int currentX = 0;
    while (charIndex < inputBuffer.size() && currentX < displayPosX && !(currentX == displayPosX - 1 && getPrintWidth(inputBuffer[charIndex]) == 2)) {
        currentX += getPrintWidth(inputBuffer[charIndex]);
        ++charIndex;
    }

    return charIndex;
}

Size LineTextInput::getMinimumSize() const {
    return Size{5, 1};
}
