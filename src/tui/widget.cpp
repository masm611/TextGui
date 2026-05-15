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

#include "widget.hpp"

#include "tui.hpp"
#include "painter.hpp"

Widget::Widget(Widget* parent) {
    this->parent = parent;
}

Widget::~Widget() {
    Tui* tui = Tui::instance();
    if (tui != nullptr) {
        tui->removeRootWidget(this);
    }

    if (parent != nullptr) {
        parent->removeChild(this);
    }

    for (Widget* child : children) {
        delete child;
    }
}

void Widget::draw() {
    if (!isVisible())
        return;

    if (bgColorEnabled) {
        Painter painter(this);
        painter.fill(Rect(0, 0, geometry().width, geometry().height), bgColor);
    }

    for (int i = 0; i < children.size(); ++i) {
        Widget* child = children[i];
        if (child->isVisible())
            child->draw();
    }
}

void Widget::setParent(Widget* newParent) {
    if (visible) {
        hide();
        this->parent = newParent;
        show();
    } else
        this->parent = newParent;
}

void Widget::addChild(Widget* child) {
    // 查重
    for (int i = 0; i < children.size(); ++i)
        if (children[i] == child)
            return;

    children.push_back(child);
}

void Widget::removeChild(Widget* child) {
    for (int i = 0; i < children.size(); ++i)
        if (children[i] == child) {
            children.erase(children.begin() + i);
            break;
        }
}

void Widget::setBackgroundColor(const Color& color) {
    bgColor = color;
    bgColorEnabled = true;
}

void Widget::disableBackgroundColor() {
    bgColorEnabled = false;
}

int Widget::addEventListener(const std::string& eventName, std::any callback) {
    if (eventName == "mouseEvent") {
        if (callback.type() != typeid(std::function<void(const MouseEvent&)>))
            throw std::invalid_argument("回调函数类型错误，期望 std::function<void(const MouseEvent&)>");

        int newId = ((mouseEventListeners.size() == 0) ? 1 : std::get<1>(*(mouseEventListeners.end() - 1)) + 1);
        mouseEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void(const MouseEvent&)>>(callback), newId));
        return newId;
    } else if (eventName == "mouseClicked") {
        if (callback.type() != typeid(std::function<void(const MouseEvent&)>))
            throw std::invalid_argument("回调函数类型错误，期望 std::function<void(const MouseEvent&)>");

        int newId = ((mouseClickedEventListeners.size() == 0) ? 1 : std::get<1>(*(mouseClickedEventListeners.end() - 1)) + 1);
        mouseClickedEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void(const MouseEvent&)>>(callback), newId));
        return newId;
    } else if (eventName == "mouseMoved") {
        if (callback.type() != typeid(std::function<void(const MouseEvent&)>))
            throw std::invalid_argument("回调函数类型错误，期望 std::function<void(const MouseEvent&)>");

        int newId = ((mouseMovedEventListeners.size() == 0) ? 1 : std::get<1>(*(mouseMovedEventListeners.end() - 1)) + 1);
        mouseMovedEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void(const MouseEvent&)>>(callback), newId));
        return newId;
    } else if (eventName == "mouseDrag") {
        if (callback.type() != typeid(std::function<void(const MouseEvent&)>))
            throw std::invalid_argument("回调函数类型错误，期望 std::function<void(const MouseEvent&)>");

        int newId = ((mouseDragEventListeners.size() == 0) ? 1 : std::get<1>(*(mouseDragEventListeners.end() - 1)) + 1);
        mouseDragEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void(const MouseEvent&)>>(callback), newId));
        return newId;
    } else if (eventName == "keyboardEvent") {
        if (callback.type() != typeid(std::function<void(const KeyboardEvent&)>))
            throw std::invalid_argument("回调函数类型错误，期望 std::function<void(const KeyboardEvent&)>");

        int newId = ((keyboardEventListeners.size() == 0) ? 1 : std::get<1>(*(keyboardEventListeners.end() - 1)) + 1);
        keyboardEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void(const KeyboardEvent&)>>(callback), newId));
        return newId;
    } else
        throw std::invalid_argument("不支持的事件类型");
}

void Widget::removeEventListener(const std::string& eventName, int listenerId) {
    if (eventName == "mouseEvent") {
        for (int i = 0; i < mouseEventListeners.size(); ++i)
            if (std::get<1>(mouseEventListeners[i]) == listenerId) {
                mouseEventListeners.erase(mouseEventListeners.begin() + i);
                break;
            }
    } else if (eventName == "mouseClicked") {
        for (int i = 0; i < mouseClickedEventListeners.size(); ++i)
            if (std::get<1>(mouseClickedEventListeners[i]) == listenerId) {
                mouseClickedEventListeners.erase(mouseClickedEventListeners.begin() + i);
                break;
            }
    } else if (eventName == "mouseMoved") {
        for (int i = 0; i < mouseMovedEventListeners.size(); ++i)
            if (std::get<1>(mouseMovedEventListeners[i]) == listenerId) {
                mouseMovedEventListeners.erase(mouseMovedEventListeners.begin() + i);
                break;
            }
    } else if (eventName == "mouseDrag") {
        for (int i = 0; i < mouseDragEventListeners.size(); ++i)
            if (std::get<1>(mouseDragEventListeners[i]) == listenerId) {
                mouseDragEventListeners.erase(mouseDragEventListeners.begin() + i);
                break;
            }
    } else if (eventName == "keyboardEvent") {
        for (int i = 0; i < keyboardEventListeners.size(); ++i)
            if (std::get<1>(keyboardEventListeners[i]) == listenerId) {
                keyboardEventListeners.erase(keyboardEventListeners.begin() + i);
                break;
            }
    } else
        throw std::invalid_argument("不支持的事件类型");
}

EventProcessResult Widget::mouseEvent(const MouseEvent& event) {
    for (auto& listener : mouseEventListeners)
        std::get<0>(listener)(event);

    if (event.eventType == MouseEventType::Click) {
        for (auto& listener : mouseClickedEventListeners)
            std::get<0>(listener)(event);
    } else if (event.eventType == MouseEventType::Move) {
        for (auto& listener : mouseMovedEventListeners)
            std::get<0>(listener)(event);
    } else if (event.eventType == MouseEventType::Drag) {
        for (auto& listener : mouseDragEventListeners)
            std::get<0>(listener)(event);
    }

    return EventProcessResult::PassDown;
}

EventProcessResult Widget::keyboardEvent(const KeyboardEvent& event) {
    for (auto& listener : keyboardEventListeners)
        std::get<0>(listener)(event);

    return EventProcessResult::PassDown;
}

void Widget::deleteLater() {
    Tui* tui = Tui::instance();
    if (tui != nullptr)
        tui->deleteWidgetLater(this);
}

void Widget::show() {
    visible = true;

    if (parent != nullptr)
        parent->addChild(this);
    else {
        Tui* tui = Tui::instance();
        if (tui != nullptr)
            tui->addRootWidget(this);
    }
}

void Widget::hide() {
    visible = false;

    Tui* tui = Tui::instance();
    if (tui != nullptr && tui->isFocusedWidget(this))
        tui->focusWidget(nullptr);

    if (parent != nullptr)
        parent->removeChild(this);
    else
        tui->removeRootWidget(this);
}
