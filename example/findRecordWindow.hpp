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
#include "tui/verticalLayoutWidget.hpp"
#include "tui/horizontalLayoutWidget.hpp"
#include "tui/label.hpp"
#include "tui/lineTextInput.hpp"
#include "tui/button.hpp"
#include "tui/marginContainerWidget.hpp"

#include "awardRecord.hpp"

enum FindFilterBy {
    ByAccountNo = 1 << 0,
    ByCompetitionName = 1 << 1,
    ByAwardLevel = 1 << 2
};

class FindRecordWindow : public Window {
public:
    FindRecordWindow(Widget* parent = nullptr);

    virtual int addEventListener(const std::string& eventName, std::any callback) override;
    virtual void removeEventListener(const std::string& eventName, int listenerId) override;

    virtual void hide() override;
    void clearInput();

    void setFindFilter(FindFilterBy filter);

    void setSubmitBtnText(const std::string& text);

    // 事件绑定
    void btnSubmitClicked();
    void btnCancelClicked();

private:
    MaximizeContainerWidget _mc{this};
    MarginContainerWidget _edgeMargin{&_mc};
    VerticalLayoutWidget _vc{&_edgeMargin};

    HorizontalLayoutWidget _hc0{&_vc};
    Label _accountNoLabel = Label("学号     ", &_hc0);
    LineTextInput _accountNoInput = LineTextInput(&_hc0);

    HorizontalLayoutWidget _hc1{&_vc};
    Label _competitionNameLabel = Label("比赛名称 ", &_hc1);
    LineTextInput _competitionNameInput = LineTextInput(&_hc1);

    HorizontalLayoutWidget _hc2{&_vc};
    Label _awardLevelLabel = Label("获奖级别 ", &_hc2);
    LineTextInput _awardLevelInput = LineTextInput(&_hc2);

    HorizontalLayoutWidget _hc3{&_vc};
    Button _submitBtn = Button("查询", &_hc3);
    Widget _submitBtnSpacer = Widget(&_hc3);
    Button _cancelBtn = Button("取消", &_hc3);

    std::vector<std::tuple<std::function<void(const AwardRecord::AwardItem&)>, int>> submitEventListeners;

    FindFilterBy findFilter = (FindFilterBy)((int)ByAccountNo | (int)ByCompetitionName | (int)ByAwardLevel);
};