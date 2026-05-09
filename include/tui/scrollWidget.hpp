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

class ScrollWidget : public Widget {
public:
    ScrollWidget(Widget* parent = nullptr);

    void draw() override;
    virtual Rect getChildDrawableArea(const Widget* childWidget) const override;
    virtual EventProcessResult mouseEvent(const MouseEvent& event) override;
    virtual EventProcessResult keyboardEvent(const KeyboardEvent& event) override;

    void setScrollable(bool vertical, bool horizontal);
    void setOffsets(int verticalOffset, int horizontalOffset);

    void scrollToBottom();

private:
    std::tuple<int, int> getContentSize() const;
    void ensureScrollOffsetBoundary();

    bool verticalScrollable = false;
    bool horizontalScrollable = false;

    int verticalScrollOffset = 0;
    int horizontalScrollOffset = 0;

    int sliderX = 0;
    int sliderHeight = 0;
    int sliderY = 0;
    int sliderWidth = 0;
};