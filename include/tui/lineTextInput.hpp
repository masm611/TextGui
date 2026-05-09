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

#pragma once

#include "widget.hpp"
#include "eventProcessResult.hpp"
#include "utf8Utils.hpp"

#include <chrono>

class LineTextInput : public Widget {
public:
    LineTextInput(Widget* parent = nullptr);

    virtual void draw() override;

    std::string getText();
    void setText(const std::string& str);

    virtual EventProcessResult keyboardEvent(const KeyboardEvent& event) override;
    virtual EventProcessResult mouseEvent(const MouseEvent& event) override;

    virtual int addEventListener(const std::string& eventName, std::any callback) override;

    void ensureCursorVisible();

    int getU8cIndexByDisplayPos(int displayPosX);

    virtual Size getMinimumSize() const override;

private:
    std::vector<Utf8Char> inputBuffer;
    int cursorPosition = 0;
    int displayStartPosition = 0;

    std::chrono::steady_clock::time_point lastBlinkTime = std::chrono::steady_clock::now() - std::chrono::milliseconds(9999);
    bool cursorVisible = true;

    std::vector<std::tuple<std::function<void(const std::string&)>, int>> textChangedEventListeners;
};