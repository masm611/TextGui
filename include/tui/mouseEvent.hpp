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

enum class MouseEventType {
    Move,
    Click,
    DoubleClick,
    Drag,

    LeftButtonDown,
    LeftButtonUp,
    RightButtonDown,
    RightButtonUp,

    WheelUp,
    WheelDown,

    Leave,

    All,
};

class MouseEvent {
public:
    short x;
    short y;
    bool leftButtonPressed;
    bool rightButtonPressed;

    MouseEventType eventType;

    short drag_lastX;
    short drag_lastY;
    short drag_beginX;
    short drag_beginY;
};