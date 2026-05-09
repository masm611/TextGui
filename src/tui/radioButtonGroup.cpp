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

#include "radioButtonGroup.hpp"

RadioButtonGroup::~RadioButtonGroup() {
    for (int i = 0; i < listeners.size(); ++i) {
        RadioButton* button = std::get<0>(listeners[i]);
        int id = std::get<1>(listeners[i]);
        button->removeEventListener("checkedChanged", id);
    }
}

void RadioButtonGroup::addButton(RadioButton* button, int id) {
    buttons.push_back(button);
    buttonIdx[button] = id;

    int listenerId = button->addEventListener("checkedChanged", std::function<void(bool)>([this, button, id](bool checked) {
                                                  if (checked) {
                                                      for (int i = 0; i < buttons.size(); ++i)
                                                          if (buttons[i] != button)
                                                              buttons[i]->setChecked(false);
                                                  }
                                              }));
    listeners.push_back(std::make_tuple(button, listenerId));
}

void RadioButtonGroup::removeButton(RadioButton* button) {
    for (int i = 0; i < buttons.size(); ++i)
        if (buttons[i] == button) {
            buttons.erase(buttons.begin() + i);
            break;
        }

    buttonIdx.erase(button);

    for (int i = 0; i < listeners.size(); ++i)
        if (std::get<0>(listeners[i]) == button) {
            button->removeEventListener("checkedChanged", std::get<1>(listeners[i]));
            listeners.erase(listeners.begin() + i);
            break;
        }
}

RadioButton* RadioButtonGroup::getCheckedButton() const {
    for (int i = 0; i < buttons.size(); ++i)
        if (buttons[i]->isChecked())
            return buttons[i];

    return nullptr;
}
