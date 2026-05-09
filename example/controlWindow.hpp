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
#include "tui/button.hpp"
#include "tui/horizontalLayoutWidget.hpp"
#include "tui/verticalLayoutWidget.hpp"
#include "tui/maximizeContainerWidget.hpp"
#include "tui/marginContainerWidget.hpp"

#include "infoTableWindow.hpp"
#include "newRecordWindow.hpp"
#include "findRecordWindow.hpp"

#include <list>

class ControlWindow : public Window {
public:
    ControlWindow();

    virtual EventProcessResult mouseEvent(const MouseEvent& event) override;

    // 按钮事件绑定
public:
    void btnShowAllMsgClicked();
    void btnInsertElementClicked();
    void btnDeleteElementClicked();
    void btnDeleteCompetitionClicked();
    void btnFindMsgByStuNOClicked();
    void btnFindMsgByCompetitionClicked();
    void btnEmptyListClicked();
    void btnEmptyFileDataClicked();
    void btnSaveAllMsgToFileClicked();
    void btnDelRecordByStudentNoClicked();

    void newRecordWindow_submitEvent(const AwardRecord::AwardItem& newItem);

private:
    MaximizeContainerWidget _controlWindow_mc;
    MarginContainerWidget _gridEdgeMargin;
    VerticalLayoutWidget _controlWindow_mc_vc;
    HorizontalLayoutWidget _controlWindow_mc_vc_hc0;
    MarginContainerWidget _showAllMsgBtn_margin;
    Button _showAllMsgBtn;
    MarginContainerWidget _insetElementBtn_margin;
    Button _insetElementBtn;
    MarginContainerWidget _delElementBtn_margin;
    Button _delElementBtn;
    HorizontalLayoutWidget _controlWindow_mc_vc_hc1;
    MarginContainerWidget _delCompetitionBtn_margin;
    Button _delCompetitionBtn;
    MarginContainerWidget _findMsgByStuNOBtn_margin;
    Button _findMsgByStuNOBtn;
    MarginContainerWidget _findMsgByCompetitionBtn_margin;
    Button _findMsgByCompetitionBtn;
    HorizontalLayoutWidget _controlWindow_mc_vc_hc2;
    MarginContainerWidget _emptyListBtn_margin;
    Button _emptyListBtn;
    MarginContainerWidget _emptyFileDataBtn_margin;
    Button _emptyFileDataBtn;
    MarginContainerWidget _saveAllMsgTofileBtn_margin;
    Button _saveAllMsgTofileBtn;
    MarginContainerWidget _delRecordByStudenNoBtn_margin{&_controlWindow_mc_vc_hc2};
    Button _delRecordByStudenNoBtn{"根据学生ID删除获奖信息", &_delRecordByStudenNoBtn_margin};

    InfoTableWindow _infoTableWindow;
    NewRecordWindow _newRecordWindow;
    FindRecordWindow _findRecordWindow;

    int findRecordWindow_submitEventListenerId = -1;
};
