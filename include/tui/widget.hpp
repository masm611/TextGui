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

#include "canvas.hpp"
#include "mouseEvent.hpp"
#include "eventProcessResult.hpp"
#include "keyboardEvent.hpp"
#include "color.hpp"
#include "rect.hpp"
#include "sizePolicy.hpp"
#include "size.hpp"

#include <vector>

#include <any>
#include <string>
#include <functional>

class Widget {
public:
    Widget(Widget* parent = nullptr);
    virtual ~Widget();

    virtual void draw();

    virtual Rect geometry() const { return _geometry; }
    virtual void setGeometry(const Rect& newGeometry) { _geometry = newGeometry; }

    Widget* getParent() const { return parent; }
    void setParent(Widget* newParent);

    const std::vector<Widget*>& getChildren() const { return children; }
    void addChild(Widget* child);
    void removeChild(Widget* child);

    virtual Rect getChildDrawableArea(const Widget* childWidget) const { return Rect(0, 0, geometry().width, geometry().height); }

    void setBackgroundColor(const Color& color);
    void disableBackgroundColor();

    virtual int addEventListener(const std::string& eventName, std::any callback);
    virtual void removeEventListener(const std::string& eventName, int listenerId);

    // 通用事件接收
    virtual EventProcessResult mouseEvent(const MouseEvent& event);
    virtual EventProcessResult keyboardEvent(const KeyboardEvent& event);

    void deleteLater();

    void show();
    virtual void hide();
    bool isVisible() const { return visible; }

    virtual SizePolicy getVerticalSizePolicy() const { return verticalSizePolicy; }
    virtual SizePolicy getHorizontalSizePolicy() const { return horizontalSizePolicy; }

    void setVerticalSizePolicy(SizePolicy policy) { verticalSizePolicy = policy; }
    void setHorizontalSizePolicy(SizePolicy policy) { horizontalSizePolicy = policy; }

    virtual Size getMinimumSize() const { return minSize; }
    virtual void setMinimumSize(const Size& newMinSize) { minSize = newMinSize; }

private:
    Widget* parent = nullptr;
    std::vector<Widget*> children;
    Rect _geometry;

    bool bgColorEnabled = false;
    Color bgColor;

    std::vector<std::tuple<std::function<void(const MouseEvent&)>, int>> mouseEventListeners;
    std::vector<std::tuple<std::function<void(const MouseEvent&)>, int>> mouseClickedEventListeners;
    std::vector<std::tuple<std::function<void(const MouseEvent&)>, int>> mouseMovedEventListeners;
    std::vector<std::tuple<std::function<void(const MouseEvent&)>, int>> mouseDragEventListeners;
    std::vector<std::tuple<std::function<void(const KeyboardEvent&)>, int>> keyboardEventListeners;

    bool visible = false;

    SizePolicy verticalSizePolicy = SizePolicy::Fixed;
    SizePolicy horizontalSizePolicy = SizePolicy::Fixed;

    Size minSize = {0, 0};
};