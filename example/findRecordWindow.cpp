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

#include "findRecordWindow.hpp"

#include "tui/messagebox.hpp"

FindRecordWindow::FindRecordWindow(Widget* parent) : Window(parent) {
    setGeometry(Rect(40, 3, 50, 10));
    setTitle("查询记录");

    _mc.show();
    _edgeMargin.setMargin(1, 1, 1, 1);
    _edgeMargin.show();
    _vc.setHorizontalSizePolicy(SizePolicy::Expanding);
    _vc.setVerticalSizePolicy(SizePolicy::Expanding);
    _vc.show();

    _hc0.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc0.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc0.show();
    _accountNoLabel.setWordWrap(false);
    _accountNoLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _accountNoLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _accountNoLabel.show();
    _accountNoInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _accountNoInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _accountNoInput.show();

    _hc1.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc1.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc1.show();
    _competitionNameLabel.setWordWrap(false);
    _competitionNameLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _competitionNameLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _competitionNameLabel.show();
    _competitionNameInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _competitionNameInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _competitionNameInput.show();

    _hc2.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc2.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc2.show();
    _awardLevelLabel.setWordWrap(false);
    _awardLevelLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _awardLevelLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _awardLevelLabel.show();
    _awardLevelInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _awardLevelInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _awardLevelInput.show();

    _hc3.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc3.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc3.show();
    _submitBtn.setHorizontalSizePolicy(SizePolicy::Expanding);
    _submitBtn.setVerticalSizePolicy(SizePolicy::Shrink);
    _submitBtn.show();
    _submitBtnSpacer.setHorizontalSizePolicy(SizePolicy::Shrink);
    _submitBtnSpacer.setVerticalSizePolicy(SizePolicy::Shrink);
    _submitBtnSpacer.setMinimumSize(Size(1, 0));
    _submitBtnSpacer.show();
    _cancelBtn.setHorizontalSizePolicy(SizePolicy::Expanding);
    _cancelBtn.setVerticalSizePolicy(SizePolicy::Shrink);
    _cancelBtn.show();

    // 事件绑定
    _submitBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&FindRecordWindow::btnSubmitClicked, this)));
    _cancelBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&FindRecordWindow::btnCancelClicked, this)));

    _awardLevelInput.addEventListener("textChanged", std::function<void(const std::string&)>([this](const std::string& newText) {
                                          if (newText.size() > 1) {
                                              std::string newStr = std::string(1, newText[newText.size() - 1]);

                                              if (newStr[0] >= 'a' && newStr[0] <= 'e') {
                                                  newStr[0] = newStr[0] - 'a' + 'A';
                                                  _awardLevelInput.setText(newStr);
                                              } else if (newStr[0] < 'A' || newStr[0] > 'E') {
                                                  _awardLevelInput.setText("");
                                              }
                                          } else if (newText.size() == 1) {
                                              if (newText[0] >= 'a' && newText[0] <= 'e') {
                                                  _awardLevelInput.setText(std::string(1, newText[0] - 'a' + 'A'));
                                              } else if (newText[0] < 'A' || newText[0] > 'E') {
                                                  _awardLevelInput.setText("");
                                              }
                                          }
                                      }));
}

int FindRecordWindow::addEventListener(const std::string& eventName, std::any callback) {
    if (eventName == "submit") {
        if (callback.type() != typeid(std::function<void(const AwardRecord::AwardItem&)>))
            throw std::invalid_argument("回调函数类型错误，应该为 std::function<void(const AwardRecord::AwardItem&)>");

        int newId = ((submitEventListeners.size() == 0) ? 1 : std::get<1>(*(submitEventListeners.end() - 1)) + 1);
        submitEventListeners.push_back(std::make_tuple(std::any_cast<std::function<void(const AwardRecord::AwardItem&)>>(callback), newId));
        return newId;
    } else
        return Window::addEventListener(eventName, callback);
}

void FindRecordWindow::removeEventListener(const std::string& eventName, int listenerId) {
    if (eventName == "submit") {
        for (auto it = submitEventListeners.begin(); it != submitEventListeners.end(); ++it) {
            if (std::get<1>(*it) == listenerId) {
                submitEventListeners.erase(it);
                break;
            }
        }
    } else {
        Window::removeEventListener(eventName, listenerId);
    }
}

void FindRecordWindow::hide() {
    Window::hide();

    clearInput();
}

void FindRecordWindow::clearInput() {
    _accountNoInput.setText("");
    _competitionNameInput.setText("");
    _awardLevelInput.setText("");
}

void FindRecordWindow::setFindFilter(FindFilterBy filter) {
    findFilter = filter;

    clearInput();

    _hc0.hide();
    _hc1.hide();
    _hc2.hide();
    _hc3.hide();

    if (((findFilter & ByAccountNo) != 0)) {
        _hc0.show();
    }

    if (((findFilter & ByCompetitionName) != 0)) {
        _hc1.show();
    }

    if (((findFilter & ByAwardLevel) != 0)) {
        _hc2.show();
    }

    _hc3.show();
}

void FindRecordWindow::setSubmitBtnText(const std::string& text) {
    _submitBtn.setText(text);
}

void FindRecordWindow::btnSubmitClicked() {
    AwardRecord::AwardItem item;

    if ((findFilter & ByAccountNo) != 0) {
        try {
            item.accountNo = std::stoull(_accountNoInput.getText());
        } catch (const std::exception&) {
            MessageBoxWindow::error("输入错误", "学号输入有误，请输入正确的学号！", std::vector<std::string>{"确定"});
            return;
        }
    }

    item.competitionName = _competitionNameInput.getText();
    item.awardGrade = (_awardLevelInput.getText().size() > 0) ? _awardLevelInput.getText()[0] : '\0';

    for (auto& listener : submitEventListeners)
        std::get<0>(listener)(item);
}

void FindRecordWindow::btnCancelClicked() {
    hide();
}
