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

#include "scrollWidget.hpp"

#include "painter.hpp"

#include "tui.hpp"

ScrollWidget::ScrollWidget(Widget* parent) : Widget(parent) {
}

void ScrollWidget::draw() {
    Widget::draw();

    // 绘制滚动条
    Painter painter(this);

    int arrowBtnPrintWidth = Tui::instance()->getArrowBtnPrintWidth();

    auto [contentWidth, contentHeight] = getContentSize();
    if (verticalScrollable) {
        // 绘制垂直滚动条background
        painter.fill(Rect(geometry().width - 2, 1, 2, geometry().height - 2), Color(100, 100, 100));
        painter.drawText(Rect(geometry().width - 2, 0, 2, 1), (arrowBtnPrintWidth == 2) ? "↑" : "↑ ", Color(255, 255, 255), Color(100, 100, 100));
        painter.drawText(Rect(geometry().width - 2, geometry().height - 1, 2, 1), (arrowBtnPrintWidth == 2) ? "↓" : "↓ ", Color(255, 255, 255), Color(100, 100, 100));

        // 绘制滑块
        if (contentHeight > geometry().height - (horizontalScrollable ? 1 : 0) && contentHeight > 0) {
            int contentDrawableHeight = geometry().height - (horizontalScrollable ? 1 : 0);
            int sliderDrawableHeight = geometry().height - 2;
            sliderHeight = std::max(1, sliderDrawableHeight * sliderDrawableHeight / contentHeight);

            int undrawableContentHeight = contentHeight - contentDrawableHeight;
            int undrawableSliderHeight = sliderDrawableHeight - sliderHeight;
            sliderY = undrawableSliderHeight * verticalScrollOffset / undrawableContentHeight;

            painter.fill(Rect(geometry().width - 2, sliderY + 1, 2, sliderHeight), Color(200, 200, 200));
        }
    }

    if (horizontalScrollable) {
        // 绘制水平滚动条background
        painter.fill(Rect(2, geometry().height - 1, geometry().width - 4 - (verticalScrollable ? 2 : 0), 2), Color(100, 100, 100));
        painter.drawText(Rect(0, geometry().height - 1, 2, 1), (arrowBtnPrintWidth == 2) ? "←" : "← ", Color(255, 255, 255), Color(100, 100, 100));
        painter.drawText(Rect(geometry().width - 2 - (verticalScrollable ? 2 : 0), geometry().height - 1, 2, 1), (arrowBtnPrintWidth == 2) ? "→" : "→ ", Color(255, 255, 255), Color(100, 100, 100));

        // 绘制滑块
        if (contentWidth > geometry().width - (verticalScrollable ? 2 : 0) && contentWidth > 0) {
            int contentDrawableWidth = geometry().width - (verticalScrollable ? 2 : 0);
            int sliderDrawableWidth = geometry().width - 4 - (verticalScrollable ? 2 : 0);
            sliderWidth = std::max(2, sliderDrawableWidth * sliderDrawableWidth / contentWidth);

            int undrawableContentWidth = contentWidth - contentDrawableWidth;
            int undrawableSliderWidth = sliderDrawableWidth - sliderWidth;
            sliderX = undrawableSliderWidth * horizontalScrollOffset / undrawableContentWidth;

            painter.fill(Rect(sliderX + 2, geometry().height - 1, sliderWidth, 2), Color(200, 200, 200));
        }
    }
}

Rect ScrollWidget::getChildDrawableArea(const Widget* childWidget) const {
    Rect area = Widget::getChildDrawableArea(childWidget);

    area.x -= horizontalScrollOffset;
    area.y -= verticalScrollOffset;

    area.width += horizontalScrollOffset;
    area.height += verticalScrollOffset;

    // 为滚动条预留空间
    if (horizontalScrollable)
        area.height -= 1;
    if (verticalScrollable)
        area.width -= 2;

    if (area.width < 0)
        area.width = 0;
    if (area.height < 0)
        area.height = 0;

    return area;
}

EventProcessResult ScrollWidget::mouseEvent(const MouseEvent& event) {
    Widget::mouseEvent(event);

    bool shouldPassDown = true;

    if (event.eventType == MouseEventType::Click) {
        if (verticalScrollable) {
            // 点击上/下箭头按钮
            if (event.x >= geometry().width - 2 && event.y == 0) {
                verticalScrollOffset -= 3;
                if (verticalScrollOffset < 0)
                    verticalScrollOffset = 0;

                shouldPassDown = false;
            } else if (event.x >= geometry().width - 2 && event.y == geometry().height - 1) {
                verticalScrollOffset += 3;

                shouldPassDown = false;
            }

            // 点击滑块
            if (event.x >= geometry().width - 2 && event.y >= sliderY && event.y < sliderY + sliderHeight) {
                shouldPassDown = false;
            }

            // 点击滚动条空白处
            if (event.x >= geometry().width - 2 && event.y > 0 && event.y < geometry().height - 1 && (event.y < sliderY || event.y >= sliderY + sliderHeight)) {
                if (event.y < sliderY)
                    verticalScrollOffset -= geometry().height - 2;
                else
                    verticalScrollOffset += geometry().height - 2;

                if (verticalScrollOffset < 0)
                    verticalScrollOffset = 0;

                shouldPassDown = false;
            }
        }

        if (horizontalScrollable) {
            // 点击左/右箭头按钮
            if (event.y >= geometry().height - 1 && event.x < 2) {
                horizontalScrollOffset -= 2;
                if (horizontalScrollOffset < 0)
                    horizontalScrollOffset = 0;

                shouldPassDown = false;
            } else if (event.y >= geometry().height - 1 && event.x >= geometry().width - 2 - (verticalScrollable ? 2 : 0) && event.x < geometry().width - (verticalScrollable ? 2 : 0)) {
                horizontalScrollOffset += 2;

                shouldPassDown = false;
            }

            // 点击滑块
            if (event.y >= geometry().height - 1 && event.x >= sliderX + 2 && event.x < sliderX + 2 + sliderWidth) {
                shouldPassDown = false;
            }

            // 点击滚动条空白处
            if (event.y >= geometry().height - 1 && event.x > 2 && event.x < geometry().width - 2 - (verticalScrollable ? 2 : 0) && (event.x < sliderX + 2 || event.x >= sliderX + 2 + sliderWidth)) {
                if (event.x < sliderX + 2)
                    horizontalScrollOffset -= geometry().width - 4 - (verticalScrollable ? 2 : 0);
                else
                    horizontalScrollOffset += geometry().width - 4 - (verticalScrollable ? 2 : 0);

                if (horizontalScrollOffset < 0)
                    horizontalScrollOffset = 0;

                shouldPassDown = false;
            }
        }
    }

    // 垂直滑块拖拽
    if (verticalScrollable) {
        static int verticalDragStartY = -1;
        if (!event.leftButtonPressed)
            verticalDragStartY = -1;

        if (event.eventType == MouseEventType::Drag) {
            if (event.drag_beginX >= geometry().width - 2 && event.drag_beginX < geometry().width && event.drag_beginY >= sliderY && event.drag_beginY <= sliderY + sliderHeight)
                if (verticalDragStartY == -1)
                    verticalDragStartY = sliderY;

            if (verticalDragStartY != -1) {
                auto [contentWidth, contentHeight] = getContentSize();

                // 计算滑块位置
                sliderY = verticalDragStartY + (event.y - event.drag_beginY);

                // 根据滑块位置反推offset
                int contentDrawableHeight = geometry().height - (horizontalScrollable ? 1 : 0);
                int sliderDrawableHeight = geometry().height - 2;

                int undrawableContentHeight = contentHeight - contentDrawableHeight;
                int undrawableSliderHeight = sliderDrawableHeight - sliderHeight;

                verticalScrollOffset = sliderY * undrawableContentHeight / undrawableSliderHeight;

                shouldPassDown = false;
            }
        }
    }

    // 水平滑块拖拽
    if (horizontalScrollable) {
        static int horizontalDragStartX = -1;
        if (!event.leftButtonPressed)
            horizontalDragStartX = -1;

        if (event.eventType == MouseEventType::Drag) {
            if (event.drag_beginY >= geometry().height - 1 && event.drag_beginY < geometry().height && event.drag_beginX >= sliderX + 2 && event.drag_beginX < sliderX + 2 + sliderWidth)
                if (horizontalDragStartX == -1)
                    horizontalDragStartX = sliderX;

            if (horizontalDragStartX != -1) {
                auto [contentWidth, contentHeight] = getContentSize();

                // 计算滑块位置
                sliderX = horizontalDragStartX + (event.x - event.drag_beginX);

                // 根据滑块位置反推offset
                int contentDrawableWidth = geometry().width - (verticalScrollable ? 2 : 0);
                int sliderDrawableWidth = geometry().width - 4 - (verticalScrollable ? 2 : 0);

                int undrawableContentWidth = contentWidth - contentDrawableWidth;
                int undrawableSliderWidth = sliderDrawableWidth - sliderWidth;

                horizontalScrollOffset = sliderX * undrawableContentWidth / undrawableSliderWidth;

                shouldPassDown = false;
            }
        }
    }

    if (event.eventType == MouseEventType::WheelDown) {
        if (Tui::instance()->isAltPressed())
            horizontalScrollOffset += 7;
        else
            verticalScrollOffset += 3;

        shouldPassDown = false;
    } else if (event.eventType == MouseEventType::WheelUp) {
        if (Tui::instance()->isAltPressed()) {
            horizontalScrollOffset -= 7;
            if (horizontalScrollOffset < 0)
                horizontalScrollOffset = 0;
        } else {
            verticalScrollOffset -= 3;
            if (verticalScrollOffset < 0)
                verticalScrollOffset = 0;
        }

        shouldPassDown = false;
    }

    ensureScrollOffsetBoundary();

    if (shouldPassDown)
        return EventProcessResult::PassDown;

    return EventProcessResult::Finished;
}

EventProcessResult ScrollWidget::keyboardEvent(const KeyboardEvent& event) {
    return EventProcessResult::PassDown;
}

std::tuple<int, int> ScrollWidget::getContentSize() const {
    int maxWidth = 0;
    int maxHeight = 0;

    for (auto child : getChildren()) {
        Rect childGeom = child->geometry();
        if (childGeom.x + childGeom.width > maxWidth)
            maxWidth = childGeom.x + childGeom.width;
        if (childGeom.y + childGeom.height > maxHeight)
            maxHeight = childGeom.y + childGeom.height;
    }

    return {maxWidth, maxHeight};
}

void ScrollWidget::ensureScrollOffsetBoundary() {
    auto [contentWidth, contentHeight] = getContentSize();

    if (horizontalScrollOffset > contentWidth - geometry().width + ((verticalScrollable) ? 2 : 0))
        horizontalScrollOffset = std::max(0, contentWidth - geometry().width + ((verticalScrollable) ? 2 : 0));
    if (verticalScrollOffset > contentHeight - geometry().height + ((horizontalScrollable) ? 1 : 0))
        verticalScrollOffset = std::max(0, contentHeight - geometry().height + ((horizontalScrollable) ? 1 : 0));

    if (horizontalScrollOffset < 0)
        horizontalScrollOffset = 0;

    if (verticalScrollOffset < 0)
        verticalScrollOffset = 0;

    if (!verticalScrollable)
        verticalScrollOffset = 0;
    if (!horizontalScrollable)
        horizontalScrollOffset = 0;
}

void ScrollWidget::setScrollable(bool vertical, bool horizontal) {
    verticalScrollable = vertical;
    horizontalScrollable = horizontal;

    ensureScrollOffsetBoundary();
}

void ScrollWidget::setOffsets(int verticalOffset, int horizontalOffset) {
    verticalScrollOffset = verticalOffset;
    horizontalScrollOffset = horizontalOffset;

    ensureScrollOffsetBoundary();
}

void ScrollWidget::scrollToBottom() {
    auto [contentWidth, contentHeight] = getContentSize();
    verticalScrollOffset = contentHeight;

    ensureScrollOffsetBoundary();
}
