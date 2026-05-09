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

#include "radioButton.hpp"

#include "painter.hpp"
#include "tui.hpp"

void RadioButton::draw() {
    Painter painter(this);

    int radioBtnSize = Tui::instance()->getRadioBtnPrintWidth();

    painter.drawText(Rect(0, 0, radioBtnSize, 1), checked ? "●" : "○", Color(255, 255, 255));
    painter.drawText(Rect(radioBtnSize, 0, geometry().width - radioBtnSize, geometry().height), text, Color(255, 255, 255));
}

EventProcessResult RadioButton::mouseEvent(const MouseEvent& event) {
    if (event.eventType == MouseEventType::Click) {
        if (!checked) {
            setChecked(true);
        }
        return EventProcessResult::Finished;
    }

    return EventProcessResult::PassDown;
}

int RadioButton::addEventListener(const std::string& eventName, std::any callback) {
    if (eventName == "checkedChanged") {
        if (callback.type() != typeid(std::function<void(bool)>))
            throw std::invalid_argument("回调函数类型错误，应该为 std::function<void(bool)>");

        int newId = ((checkedChangedEventListeners.size() == 0) ? 1 : std::get<1>(*(checkedChangedEventListeners.end() - 1)) + 1);
        checkedChangedEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void(bool)>>(callback), newId));
        return newId;
    } else {
        return Widget::addEventListener(eventName, callback);
    }
}

void RadioButton::removeEventListener(const std::string& eventName, int listenerId) {
    if (eventName == "checkedChanged") {
        for (int i = 0; i < checkedChangedEventListeners.size(); ++i)
            if (std::get<1>(checkedChangedEventListeners[i]) == listenerId) {
                checkedChangedEventListeners.erase(checkedChangedEventListeners.begin() + i);
                break;
            }
    } else {
        Widget::removeEventListener(eventName, listenerId);
    }
}

Size RadioButton::getMinimumSize() const {
    int textWidth = getPrintWidth(text);
    return Size{2 + 1 + textWidth, 1};
}

void RadioButton::setChecked(bool newChecked) {
    if (checked != newChecked) {
        checked = newChecked;

        for (auto& listener : checkedChangedEventListeners)
            std::get<0>(listener)(checked);
    }
}
