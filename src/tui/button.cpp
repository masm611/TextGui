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

#include "button.hpp"

#include "painter.hpp"
#include "utf8Utils.hpp"

#include "tui.hpp"

void Button::draw() {
    Widget::draw();

    Painter painter(this);

    Color buttonColor = !(isPressed && Tui::instance()->isMouseHoverWidget(this)) ? Color(90, 90, 90) : Color(30, 30, 30);
    painter.fill(Rect(0, 0, geometry().width, geometry().height), buttonColor);

    auto limitedText = limitStrPrintWidth(text, geometry().width);
    short textPrintWidth = getPrintWidth(limitedText);
    short textX = (geometry().width - textPrintWidth) / 2;
    short textY = geometry().height / 2;
    painter.drawText(Rect(textX, textY, geometry().width - textX, geometry().height - textY), limitedText, Color(255, 255, 255));
}

EventProcessResult Button::mouseEvent(const MouseEvent& event) {
    Widget::mouseEvent(event);

    if (event.eventType == MouseEventType::LeftButtonUp) {
        // for (auto& listener : buttonClickedEventListeners)
        //     std::get<0>(listener)();

        isPressed = false;
        return EventProcessResult::Finished;
    }

    if (event.eventType == MouseEventType::Click) {
        for (auto& listener : buttonClickedEventListeners)
            std::get<0>(listener)();

        return EventProcessResult::Finished;
    }

    if (event.leftButtonPressed && event.x >= 0 && event.x < geometry().width && event.y >= 0 && event.y < geometry().height)
        isPressed = true;
    else
        isPressed = false;

    return EventProcessResult::PassDown;
}

int Button::addEventListener(const std::string& eventName, std::any callback) {
    if (eventName == "buttonClicked") {
        if (callback.type() != typeid(std::function<void(void)>))
            throw std::invalid_argument("回调函数类型错误，期望 std::function<void(void)>");

        int newId = ((buttonClickedEventListeners.size() == 0) ? 1 : std::get<1>(*(buttonClickedEventListeners.end() - 1)) + 1);
        buttonClickedEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void(void)>>(callback), newId));
        return newId;
    } else
        return Widget::addEventListener(eventName, callback);
}

Size Button::getMinimumSize() const {
    int textWidth = getPrintWidth(text);
    int width = textWidth + 2;
    int height = 1;

    if (mannualMinSize.width != -1)
        width = mannualMinSize.width;
    if (mannualMinSize.height != -1)
        height = mannualMinSize.height;

    return Size(width, height);
}

void Button::setMinimumSize(const Size& newMinSize) {
    mannualMinSize = newMinSize;
    Widget::setMinimumSize(newMinSize);
}