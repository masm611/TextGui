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

#include "window.hpp"

#include "painter.hpp"
#include "tui.hpp"

Window::Window(Widget* parent) : Widget(parent) {
    Widget::setBackgroundColor(Color(50, 50, 50));

    for (int i = 0; i < 5; ++i)
        resizeHandle[i].setParent(this);
}

void Window::draw() {
    if (resizeMode) {
        // 调整位置大小
        Rect g = geometry();
        g.height -= 1;

        resizeHandle[0].setGeometry(Rect(0, g.height / 2 - 1, 2, 2));
        resizeHandle[1].setGeometry(Rect(0, g.height - 1, 2, 1));
        resizeHandle[2].setGeometry(Rect(g.width / 2 - 2, g.height - 1, 4, 1));
        resizeHandle[3].setGeometry(Rect(g.width - 2, g.height - 1, 2, 1));
        resizeHandle[4].setGeometry(Rect(g.width - 2, g.height / 2 - 1, 2, 2));
    }

    Widget::draw();

    Painter painter(this);

    // 绘制标题栏
    painter.fill(Rect(0, 0, geometry().width, 1), titleBgColor);
    painter.drawText(Rect(0, 0, geometry().width, 1), title, titleFgColor, titleBgColor);

    // painter.drawText(Rect(geometry().width - 2, 0, 2, 1), "×", Color(255, 255, 255), Color(255, 0, 0));
    // painter.drawText(Rect(geometry().width - 2, 0, 2, 1), "ｘ", Color(255, 255, 255), Color(255, 0, 0));
    painter.drawText(Rect(geometry().width - 2, 0, 2, 1), "  ", Color(255, 255, 255), Color(255, 0, 0));

    if (maximized)
        painter.drawText(Rect(geometry().width - 4, 0, 2, 1), "  ", Color(255, 255, 255), Color(0, 162, 232));
    else
        painter.drawText(Rect(geometry().width - 4, 0, 2, 1), "  ", Color(255, 255, 255), Color(0, 162, 232));

    if (!resizeMode)
        painter.drawText(Rect(geometry().width - 6, 0, 2, 1), "  ", Color(255, 255, 255), Color(127, 127, 127));
    else
        painter.drawText(Rect(geometry().width - 6, 0, 2, 1), "  ", Color(255, 255, 255), Color(0, 200, 0));
}

Rect Window::getChildDrawableArea(const Widget* childWidget) const {
    return Rect(0, 1, geometry().width, geometry().height - 1);
}

void Window::setTitleColor(const Color& fgColor, const Color& bgColor) {
    titleFgColor = fgColor;
    titleBgColor = bgColor;
}

void Window::setTitle(const std::string& title) {
    this->title = title;
}

EventProcessResult Window::mouseEvent(const MouseEvent& event) {
    Widget::mouseEvent(event);

    if (event.eventType == MouseEventType::Drag && event.drag_lastY == 0 && (event.drag_beginY == 0 || moving)) {
        moving = true;

        if (maximized) {
            short maximizedWindowWidth = geometry().width;

            maximized = false;

            short windowWidth = originGeometryBeforeMaximize.width;

            short zoomedDragBeginX = (short)(((double)(event.x) / (double)maximizedWindowWidth) * (double)(windowWidth));
            setGeometry(Rect(event.x - zoomedDragBeginX, event.y, originGeometryBeforeMaximize.width, originGeometryBeforeMaximize.height));

            return EventProcessResult::Finished;
        }

        setGeometry(Rect(geometry().x + event.x - event.drag_lastX, geometry().y + event.y, geometry().width, geometry().height));
        return EventProcessResult::Finished;
    }

    if (event.eventType == MouseEventType::Click && event.x >= geometry().width - 2 && event.x < geometry().width && event.y == 0) {
        // 点击了关闭按钮
        hide();

        for (const auto& listener : windowCloseEventListeners)
            std::get<0>(listener)();

        return EventProcessResult::Finished;
    }

    if (event.eventType == MouseEventType::Click && event.x >= geometry().width - 4 && event.x < geometry().width - 2 && event.y == 0) {
        // 点击了最大化/还原按钮
        if (maximized)
            unmaximizeWindow();
        else
            maximizeWindow();
        return EventProcessResult::Finished;
    }

    if (event.eventType == MouseEventType::Click && event.x >= geometry().width - 6 && event.x < geometry().width - 4 && event.y == 0) {
        // 点击了调整大小按钮
        resizeMode = !resizeMode;

        if (resizeMode)
            for (int i = 0; i < 5; ++i)
                resizeHandle[i].show();
        else
            for (int i = 0; i < 5; ++i)
                resizeHandle[i].hide();

        return EventProcessResult::Finished;
    }

    if (!event.leftButtonPressed)
        moving = false;

    if (event.eventType == MouseEventType::DoubleClick && event.y == 0 && event.x >= 0 && event.x < geometry().width - 6) {
        maximizeWindow();
        return EventProcessResult::Finished;
    }

    return EventProcessResult::PassDown;
}

int Window::addEventListener(const std::string& eventName, std::any callback) {
    if (eventName == "windowClose") {
        if (callback.type() == typeid(std::function<void()>)) {
            int newId = ((windowCloseEventListeners.size() == 0) ? 1 : std::get<1>(*(windowCloseEventListeners.end() - 1)) + 1);
            windowCloseEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void()>>(callback), newId));
            return newId;
        } else
            throw std::invalid_argument("Invalid callback type for windowClose event");
    } else
        return Widget::addEventListener(eventName, callback);
}

Rect Window::geometry() const {
    if (maximized) {
        if (Widget::getParent() == nullptr) {
            Tui* tui = Tui::instance();
            short screenWidth = tui->width();
            short screenHeight = tui->height();
            return Rect(0, 0, screenWidth, screenHeight);
        } else {
            Rect maxArea = Widget::getParent()->getChildDrawableArea(this);
            return Rect(0, 0, maxArea.width, maxArea.height);
        }
    } else
        return Widget::geometry();
}

void Window::maximizeWindow() {
    originGeometryBeforeMaximize = geometry();
    maximized = true;
}

void Window::unmaximizeWindow() {
    maximized = false;
    setGeometry(originGeometryBeforeMaximize);
}

void Window::resizeWindowHandleMouseEvent(WindowResizeHandle* handle, const MouseEvent& event) {
    if (event.eventType != MouseEventType::Drag) {
        return;
    }

    int handleIdx = -1;
    for (int i = 0; i < 5; ++i)
        if (&resizeHandle[i] == handle) {
            handleIdx = i;
            break;
        }

    if (handleIdx == -1)
        throw std::invalid_argument("Invalid resize handle");

    int deltaX = event.x - event.drag_lastX;
    int deltaY = event.y - event.drag_lastY;

    switch (handleIdx) {
        case 0:
            if (geometry().width - deltaX < 8)
                break;
            setGeometry(Rect(geometry().x + deltaX, geometry().y, geometry().width - deltaX, geometry().height));
            break;
        case 1:
            if (geometry().width - deltaX < 8 || geometry().height + deltaY < 2)
                break;
            setGeometry(Rect(geometry().x + deltaX, geometry().y, geometry().width - deltaX, geometry().height + deltaY));
            break;
        case 2:
            if (geometry().height + deltaY < 2)
                break;
            setGeometry(Rect(geometry().x, geometry().y, geometry().width, geometry().height + deltaY));
            break;
        case 3:
            if (geometry().width + deltaX < 8 || geometry().height + deltaY < 2)
                break;
            setGeometry(Rect(geometry().x, geometry().y, geometry().width + deltaX, geometry().height + deltaY));
            break;
        case 4:
            if (geometry().width + deltaX < 8)
                break;
            setGeometry(Rect(geometry().x, geometry().y, geometry().width + deltaX, geometry().height));
            break;
    }
}
