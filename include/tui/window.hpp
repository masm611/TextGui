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
#include "button.hpp"
#include "windowResizeHandle.hpp"

#include <functional>
#include <string>

class Window : public Widget {
public:
    Window(Widget* parent = nullptr);
    virtual void draw() override;
    virtual Rect getChildDrawableArea(const Widget* childWidget) const override;

    void setTitleColor(const Color& fgColor, const Color& bgColor);
    void setTitle(const std::string& title);

    virtual EventProcessResult mouseEvent(const MouseEvent& event) override;

    virtual int addEventListener(const std::string& eventName, std::any callback) override;

    virtual Rect geometry() const override;

    void maximizeWindow();
    void unmaximizeWindow();

    void resizeWindowHandleMouseEvent(WindowResizeHandle* handle, const MouseEvent& event);

private:
    Color titleBgColor = Color(255, 255, 255);
    Color titleFgColor = Color(0, 0, 0);
    std::string title = "Default Window Title";

    bool moving = false;
    bool maximized = false;
    Rect originGeometryBeforeMaximize;

    std::vector<std::tuple<std::function<void()>, int>> windowCloseEventListeners;

    bool resizeMode = false;
    WindowResizeHandle resizeHandle[5];
};