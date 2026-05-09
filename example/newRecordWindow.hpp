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

#include "tui/window.hpp"
#include "tui/maximizeContainerWidget.hpp"
#include "tui/marginContainerWidget.hpp"
#include "tui/verticalLayoutWidget.hpp"
#include "tui/horizontalLayoutWidget.hpp"
#include "tui/label.hpp"
#include "tui/lineTextInput.hpp"
#include "tui/button.hpp"
#include "tui/radioButton.hpp"
#include "tui/radioButtonGroup.hpp"

#include "awardRecord.hpp"

class NewRecordWindow : public Window {
public:
    NewRecordWindow(Widget* parent = nullptr);

    virtual int addEventListener(const std::string& eventName, std::any callback) override;

    void clearInput();
    void hide() override;

    // 事件绑定
    void btnSubmitClicked();
    void btnCancelClicked();

private:
    MaximizeContainerWidget _mc{this};
    MarginContainerWidget _edgeMargin{&_mc};
    VerticalLayoutWidget _vc{&_edgeMargin};

    HorizontalLayoutWidget _hc0 = HorizontalLayoutWidget(&_vc);
    Label _studentNameLabel = Label("姓名     ", &_hc0);
    LineTextInput _studentNameInput = LineTextInput(&_hc0);

    HorizontalLayoutWidget _hc1 = HorizontalLayoutWidget(&_vc);
    Label _accountNoLabel = Label("学号     ", &_hc1);
    LineTextInput _accountNoInput = LineTextInput(&_hc1);

    HorizontalLayoutWidget _hc2 = HorizontalLayoutWidget(&_vc);
    Label _sexLabel = Label("性别     ", &_hc2);
    RadioButton _maleButton = RadioButton("男", &_hc2);
    RadioButton _femaleButton = RadioButton("女", &_hc2);

    HorizontalLayoutWidget _hc3 = HorizontalLayoutWidget(&_vc);
    Label _professionLabel = Label("专业     ", &_hc3);
    LineTextInput _professionInput = LineTextInput(&_hc3);

    HorizontalLayoutWidget _hc4 = HorizontalLayoutWidget(&_vc);
    Label _classNameLabel = Label("班级     ", &_hc4);
    LineTextInput _classNameInput = LineTextInput(&_hc4);

    HorizontalLayoutWidget _hc5 = HorizontalLayoutWidget(&_vc);
    Label _competitionNameLabel = Label("比赛名称 ", &_hc5);
    LineTextInput _competitionNameInput = LineTextInput(&_hc5);

    HorizontalLayoutWidget _hc6 = HorizontalLayoutWidget(&_vc);
    Label _competitionGradeLabel = Label("赛事级别 ", &_hc6);
    LineTextInput _competitionGradeInput = LineTextInput(&_hc6);

    HorizontalLayoutWidget _hc7 = HorizontalLayoutWidget(&_vc);
    Label _awardGradeLabel = Label("获奖级别 ", &_hc7);
    LineTextInput _awardGradeInput = LineTextInput(&_hc7);

    HorizontalLayoutWidget _buttonHc = HorizontalLayoutWidget(&_vc);
    Button _submitBtn = Button("新增", &_buttonHc);
    Widget _buttonMargin = Widget(&_buttonHc);
    Button _cancelBtn = Button("取消", &_buttonHc);

    RadioButtonGroup _sexGroup = RadioButtonGroup();
    std::vector<std::tuple<std::function<void(const AwardRecord::AwardItem&)>, int>> newRecordListeners;
};