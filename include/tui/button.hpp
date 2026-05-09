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
#include "mouseEvent.hpp"
#include "eventProcessResult.hpp"

class Button : public Widget {
public:
    Button(Widget* parent = nullptr) : Widget(parent) {};
    Button(const std::string& text, Widget* parent = nullptr) : Widget(parent), text(text) {};

    virtual void draw() override;

    virtual EventProcessResult mouseEvent(const MouseEvent& event) override;
    virtual int addEventListener(const std::string& eventName, std::any callback) override;

    void setText(const std::string& newText) { text = newText; }
    std::string_view getText() const { return text; }

    virtual Size getMinimumSize() const override;
    void setMinimumSize(const Size& newMinSize) override;

private:
    std::string text;
    std::vector<std::tuple<std::function<void(void)>, int>> buttonClickedEventListeners;
    bool isPressed = false;

    Size mannualMinSize = {-1, -1};
};