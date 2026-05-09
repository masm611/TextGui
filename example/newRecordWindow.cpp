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

#include "newRecordWindow.hpp"

#include "tui/messagebox.hpp"

NewRecordWindow::NewRecordWindow(Widget* parent) : Window(parent) {
    setGeometry(Rect(35, 2, 60, 20));

    _sexGroup.addButton(&_maleButton, 0);
    _sexGroup.addButton(&_femaleButton, 1);

    setTitle("添加新记录");

    _mc.show();
    _edgeMargin.setMargin(1, 1, 1, 1);
    _edgeMargin.show();
    _vc.setVerticalSizePolicy(SizePolicy::Expanding);
    _vc.setHorizontalSizePolicy(SizePolicy::Expanding);
    _vc.show();

    _hc0.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc0.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc0.show();
    _studentNameLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _studentNameLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _studentNameLabel.setWordWrap(false);
    _studentNameLabel.show();
    _studentNameInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _studentNameInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _studentNameInput.show();

    _hc1.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc1.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc1.show();
    _accountNoLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _accountNoLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _accountNoLabel.show();
    _accountNoInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _accountNoInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _accountNoInput.show();

    _hc2.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc2.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc2.show();
    _sexLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _sexLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _sexLabel.show();
    _maleButton.setHorizontalSizePolicy(SizePolicy::Expanding);
    _maleButton.setVerticalSizePolicy(SizePolicy::Shrink);
    _maleButton.show();
    _femaleButton.setHorizontalSizePolicy(SizePolicy::Expanding);
    _femaleButton.setVerticalSizePolicy(SizePolicy::Shrink);
    _femaleButton.show();

    _hc3.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc3.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc3.show();
    _professionLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _professionLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _professionLabel.show();
    _professionInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _professionInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _professionInput.show();

    _hc4.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc4.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc4.show();
    _classNameLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _classNameLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _classNameLabel.show();
    _classNameInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _classNameInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _classNameInput.show();

    _hc5.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc5.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc5.show();
    _competitionNameLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _competitionNameLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _competitionNameLabel.show();
    _competitionNameInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _competitionNameInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _competitionNameInput.show();

    _hc6.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc6.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc6.show();
    _competitionGradeLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _competitionGradeLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _competitionGradeLabel.show();
    _competitionGradeInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _competitionGradeInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _competitionGradeInput.show();

    _hc7.setHorizontalSizePolicy(SizePolicy::Expanding);
    _hc7.setVerticalSizePolicy(SizePolicy::Expanding);
    _hc7.show();
    _awardGradeLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
    _awardGradeLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    _awardGradeLabel.show();
    _awardGradeInput.setHorizontalSizePolicy(SizePolicy::Expanding);
    _awardGradeInput.setVerticalSizePolicy(SizePolicy::Shrink);
    _awardGradeInput.show();

    _buttonHc.setHorizontalSizePolicy(SizePolicy::Expanding);
    _buttonHc.setVerticalSizePolicy(SizePolicy::Expanding);
    _buttonHc.show();
    _submitBtn.setHorizontalSizePolicy(SizePolicy::Expanding);
    _submitBtn.setVerticalSizePolicy(SizePolicy::Shrink);
    _submitBtn.show();
    _buttonMargin.setHorizontalSizePolicy(SizePolicy::Shrink);
    _buttonMargin.setVerticalSizePolicy(SizePolicy::Shrink);
    _buttonMargin.setMinimumSize(Size(1, 0));
    _buttonMargin.show();
    _cancelBtn.setHorizontalSizePolicy(SizePolicy::Expanding);
    _cancelBtn.setVerticalSizePolicy(SizePolicy::Shrink);
    _cancelBtn.show();

    // 事件绑定
    _submitBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&NewRecordWindow::btnSubmitClicked, this)));
    _cancelBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&NewRecordWindow::btnCancelClicked, this)));

    _competitionGradeInput.addEventListener("textChanged", std::function<void(const std::string&)>([this](const std::string& newText) {
                                                if (newText.size() > 1) {
                                                    std::string newStr = std::string(1, newText[newText.size() - 1]);

                                                    if (newStr[0] >= 'a' && newStr[0] <= 'e') {
                                                        newStr[0] = newStr[0] - 'a' + 'A';
                                                        _competitionGradeInput.setText(newStr);
                                                    } else if (newStr[0] < 'A' || newStr[0] > 'E') {
                                                        _competitionGradeInput.setText("");
                                                    }
                                                } else if (newText.size() == 1) {
                                                    if (newText[0] >= 'a' && newText[0] <= 'e') {
                                                        _competitionGradeInput.setText(std::string(1, newText[0] - 'a' + 'A'));
                                                    } else if (newText[0] < 'A' || newText[0] > 'E') {
                                                        _competitionGradeInput.setText("");
                                                    }
                                                }
                                            }));

    _awardGradeInput.addEventListener("textChanged", std::function<void(const std::string&)>([this](const std::string& newText) {
                                          if (newText.size() > 1) {
                                              std::string newStr = std::string(1, newText[newText.size() - 1]);

                                              if (newStr[0] >= 'a' && newStr[0] <= 'e') {
                                                  newStr[0] = newStr[0] - 'a' + 'A';
                                                  _awardGradeInput.setText(newStr);
                                              } else if (newStr[0] < 'A' || newStr[0] > 'E') {
                                                  _awardGradeInput.setText("");
                                              }
                                          } else if (newText.size() == 1) {
                                              if (newText[0] >= 'a' && newText[0] <= 'e') {
                                                  _awardGradeInput.setText(std::string(1, newText[0] - 'a' + 'A'));
                                              } else if (newText[0] < 'A' || newText[0] > 'E') {
                                                  _awardGradeInput.setText("");
                                              }
                                          }
                                      }));
}

int NewRecordWindow::addEventListener(const std::string& eventName, std::any callback) {
    if (eventName == "newRecord") {
        if (callback.type() != typeid(std::function<void(const AwardRecord::AwardItem&)>))
            throw std::invalid_argument("回调函数类型错误，应该为 std::function<void(const AwardRecord::AwardItem&)>");

        int newId = ((newRecordListeners.size() == 0) ? 1 : std::get<1>(*(newRecordListeners.end() - 1)) + 1);
        newRecordListeners.push_back(std::make_tuple(std::any_cast<std::function<void(const AwardRecord::AwardItem&)>>(callback), newId));
        return newId;
    } else {
        return Window::addEventListener(eventName, callback);
    }
}

void NewRecordWindow::clearInput() {
    _studentNameInput.setText("");
    _accountNoInput.setText("");
    _maleButton.setChecked(false);
    _femaleButton.setChecked(false);
    _professionInput.setText("");
    _classNameInput.setText("");
    _competitionNameInput.setText("");
    _competitionGradeInput.setText("");
    _awardGradeInput.setText("");
}

void NewRecordWindow::hide() {
    Window::hide();

    clearInput();
}

void NewRecordWindow::btnSubmitClicked() {
    AwardRecord::AwardItem newItem;

    newItem.studentName = std::string(_studentNameInput.getText());

    try {
        newItem.accountNo = std::stoull(_accountNoInput.getText());
    } catch (const std::exception&) {
        MessageBoxWindow::error("错误", "学号必须为数字", std::vector<std::string>({"确定"}));
        return;
    }

    if (_sexGroup.getCheckedButton() == nullptr) {
        MessageBoxWindow::error("错误", "请选择性别", std::vector<std::string>({"确定"}));
        return;
    }
    newItem.sex = (_sexGroup.getCheckedButton() == &_maleButton) ? AwardRecord::Sex::Male : AwardRecord::Sex::Female;

    newItem.profession = _professionInput.getText();
    newItem.className = _classNameInput.getText();
    newItem.competitionName = _competitionNameInput.getText();

    if (_competitionGradeInput.getText().size() == 0 || _awardGradeInput.getText().size() == 0) {
        MessageBoxWindow::error("错误", "赛事级别和获奖级别不能为空", std::vector<std::string>({"确定"}));
        return;
    }

    newItem.competitionGrade = _competitionGradeInput.getText()[0];
    newItem.awardGrade = _awardGradeInput.getText()[0];

    for (auto& listener : newRecordListeners) {
        std::get<0>(listener)(newItem);
    }

    hide();
}

void NewRecordWindow::btnCancelClicked() {
    hide();
}
