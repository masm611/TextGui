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

#include "window.hpp"
#include "label.hpp"
#include "maximizeContainerWidget.hpp"
#include "scrollWidget.hpp"

class TerminalWindow : public Window {
public:
    TerminalWindow(Widget* parent = nullptr);
    virtual void draw() override;

    void write(const std::string_view& text);

    // virtual EventProcessResult keyboardEvent(const KeyboardEvent& event) override;

private:
    MaximizeContainerWidget _mc = MaximizeContainerWidget(this);
    ScrollWidget _scrollWidget = ScrollWidget(&_mc);
    Label outputLabel = Label("", &_scrollWidget);

    std::string outputBuffer;
};