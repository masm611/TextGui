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

#include "terminalWindow.hpp"

TerminalWindow::TerminalWindow(Widget* parent) : Window(parent) {
    _mc.show();
    _scrollWidget.setScrollable(true, true);
    _scrollWidget.show();

    outputLabel.setTextColor(Color(255, 255, 255));
    outputLabel.setAutoResize(true, true);
    outputLabel.setWordWrap(false);
    outputLabel.show();

    setTitle("终端窗口");
    setBackgroundColor(Color(0, 0, 0));
}

void TerminalWindow::draw() {
    Window::draw();
}

void TerminalWindow::write(const std::string_view& text) {
    outputBuffer += text;
    outputLabel.setText(outputBuffer);
    _scrollWidget.scrollToBottom();
}

// EventProcessResult TerminalWindow::keyboardEvent(const KeyboardEvent& event) {
//     Window::keyboardEvent(event);

//     if (event.eventType == KeyboardEventType::CharInput) {
//         if (event.keyChar == '\b') {
//             if (outputBuffer.empty())
//                 return EventProcessResult::Finished;

//             if (*(outputBuffer.end() - 1) >= 0 && *(outputBuffer.end() - 1) <= 127) {
//                 outputBuffer.pop_back();
//                 outputLabel->setText(outputBuffer);
//                 return EventProcessResult::Finished;
//             }

//             // 寻找最后一个UTF-8字符的起始位置
//             for (int i = (int)outputBuffer.size() - 1; i >= 0; --i) {
//                 if ((outputBuffer[i] & 0b11000000) != 0b10000000) {
//                     outputBuffer.erase(outputBuffer.begin() + i, outputBuffer.end());
//                     outputLabel->setText(outputBuffer);
//                     return EventProcessResult::Finished;
//                 }
//             }
//         } else {
//             outputBuffer += event.keyChar;
//             outputLabel->setText(outputBuffer);
//         }
//     }

//     return EventProcessResult::Finished;
// }
