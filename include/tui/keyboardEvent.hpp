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

#include <vector>

// 左箭头 = 37
// 上箭头 = 38
// 右箭头 = 39
// 下箭头 = 40

namespace SpecialVirtualKeyCode {
enum Code {
    LeftArrow = 37,
    UpArrow = 38,
    RightArrow = 39,
    DownArrow = 40,
};
}

enum class KeyboardEventType {
    KeyDown,
    KeyUp,
    CharInput,
    SpecialInput,  // 非字符输入

    All,
};

class KeyboardEvent {
public:
    bool ctrlPressed;
    bool shiftPressed;
    bool altPressed;

    std::vector<char> pressedKeys;
    char keyChar;
    int virtualKeyCode;
    KeyboardEventType eventType;
};