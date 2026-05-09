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

#include "controlWindow.hpp"

#include "tui/messagebox.hpp"
#include "tui/tui.hpp"

#include "awardRecord.hpp"

ControlWindow::ControlWindow()
    : _controlWindow_mc(this), _gridEdgeMargin(&_controlWindow_mc), _controlWindow_mc_vc(&_gridEdgeMargin), _controlWindow_mc_vc_hc0(&_controlWindow_mc_vc), _showAllMsgBtn_margin(&_controlWindow_mc_vc_hc0), _showAllMsgBtn("显示所有获奖信息", &_showAllMsgBtn_margin), _insetElementBtn_margin(&_controlWindow_mc_vc_hc0), _insetElementBtn("新获奖信息", &_insetElementBtn_margin), _delElementBtn_margin(&_controlWindow_mc_vc_hc0), _delElementBtn("删除获奖信息", &_delElementBtn_margin), _controlWindow_mc_vc_hc1(&_controlWindow_mc_vc), _delCompetitionBtn_margin(&_controlWindow_mc_vc_hc1), _delCompetitionBtn("根据比赛删除获奖信息", &_delCompetitionBtn_margin), _findMsgByStuNOBtn_margin(&_controlWindow_mc_vc_hc1), _findMsgByStuNOBtn("根据学生ID查询获奖信息", &_findMsgByStuNOBtn_margin), _findMsgByCompetitionBtn_margin(&_controlWindow_mc_vc_hc1), _findMsgByCompetitionBtn("根据比赛查询获奖信息", &_findMsgByCompetitionBtn_margin), _controlWindow_mc_vc_hc2(&_controlWindow_mc_vc), _emptyListBtn_margin(&_controlWindow_mc_vc_hc2), _emptyListBtn("清空获奖信息", &_emptyListBtn_margin), _emptyFileDataBtn_margin(&_controlWindow_mc_vc_hc2), _emptyFileDataBtn("清空获奖信息文件", &_emptyFileDataBtn_margin), _saveAllMsgTofileBtn_margin(&_controlWindow_mc_vc_hc2), _saveAllMsgTofileBtn("保存获奖信息到文件", &_saveAllMsgTofileBtn_margin) {
    // ui布局
    setGeometry(Rect(10, 5, 100, 20));
    setBackgroundColor(Color(0, 155, 186));
    setTitle("控制台");
    show();

    _controlWindow_mc.show();

    _gridEdgeMargin.setMargin(1, 0, 1, 2);
    _gridEdgeMargin.show();

    _controlWindow_mc_vc.show();

    _controlWindow_mc_vc_hc0.setHorizontalSizePolicy(SizePolicy::Expanding);
    _controlWindow_mc_vc_hc0.setVerticalSizePolicy(SizePolicy::Expanding);
    _controlWindow_mc_vc_hc0.show();

    _showAllMsgBtn_margin.setMargin(0, 2, 1, 0);
    _showAllMsgBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _showAllMsgBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _showAllMsgBtn_margin.show();
    _showAllMsgBtn.show();

    _insetElementBtn_margin.setMargin(0, 2, 1, 0);
    _insetElementBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _insetElementBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _insetElementBtn_margin.show();
    _insetElementBtn.show();

    _delElementBtn_margin.setMargin(0, 2, 1, 0);
    _delElementBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _delElementBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _delElementBtn_margin.show();
    _delElementBtn.show();

    _controlWindow_mc_vc_hc1.setHorizontalSizePolicy(SizePolicy::Expanding);
    _controlWindow_mc_vc_hc1.setVerticalSizePolicy(SizePolicy::Expanding);
    _controlWindow_mc_vc_hc1.show();

    _delCompetitionBtn_margin.setMargin(0, 2, 1, 0);
    _delCompetitionBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _delCompetitionBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _delCompetitionBtn_margin.show();
    _delCompetitionBtn.show();

    _findMsgByStuNOBtn_margin.setMargin(0, 2, 1, 0);
    _findMsgByStuNOBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _findMsgByStuNOBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _findMsgByStuNOBtn_margin.show();
    _findMsgByStuNOBtn.show();

    _findMsgByCompetitionBtn_margin.setMargin(0, 2, 1, 0);
    _findMsgByCompetitionBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _findMsgByCompetitionBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _findMsgByCompetitionBtn_margin.show();
    _findMsgByCompetitionBtn.show();

    _controlWindow_mc_vc_hc2.setHorizontalSizePolicy(SizePolicy::Expanding);
    _controlWindow_mc_vc_hc2.setVerticalSizePolicy(SizePolicy::Expanding);
    _controlWindow_mc_vc_hc2.show();

    _emptyListBtn_margin.setMargin(0, 2, 0, 0);
    _emptyListBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _emptyListBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _emptyListBtn_margin.show();
    _emptyListBtn.show();

    _emptyFileDataBtn_margin.setMargin(0, 2, 0, 0);
    _emptyFileDataBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _emptyFileDataBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _emptyFileDataBtn_margin.show();
    _emptyFileDataBtn.show();

    _saveAllMsgTofileBtn_margin.setMargin(0, 2, 0, 0);
    _saveAllMsgTofileBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _saveAllMsgTofileBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _saveAllMsgTofileBtn_margin.show();
    _saveAllMsgTofileBtn.show();

    _delRecordByStudenNoBtn_margin.setMargin(0, 2, 0, 0);
    _delRecordByStudenNoBtn_margin.setHorizontalSizePolicy(SizePolicy::Expanding);
    _delRecordByStudenNoBtn_margin.setVerticalSizePolicy(SizePolicy::Expanding);
    _delRecordByStudenNoBtn_margin.show();
    _delRecordByStudenNoBtn.show();

    // 事件绑定
    _showAllMsgBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnShowAllMsgClicked, this)));
    _insetElementBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnInsertElementClicked, this)));
    _delElementBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnDeleteElementClicked, this)));
    _delCompetitionBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnDeleteCompetitionClicked, this)));
    _findMsgByStuNOBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnFindMsgByStuNOClicked, this)));
    _findMsgByCompetitionBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnFindMsgByCompetitionClicked, this)));
    _emptyListBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnEmptyListClicked, this)));
    _emptyFileDataBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnEmptyFileDataClicked, this)));
    _saveAllMsgTofileBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnSaveAllMsgToFileClicked, this)));
    _delRecordByStudenNoBtn.addEventListener("buttonClicked", std::function<void(void)>(std::bind(&ControlWindow::btnDelRecordByStudentNoClicked, this)));

    _newRecordWindow.addEventListener("newRecord", std::function<void(const AwardRecord::AwardItem&)>(std::bind(&ControlWindow::newRecordWindow_submitEvent, this, std::placeholders::_1)));

    // 其它设置
    _infoTableWindow.setGeometry(Rect(30, 10, 80, 20));
    _infoTableWindow.setTitle("信息表格窗口");
}

EventProcessResult ControlWindow::mouseEvent(const MouseEvent& event) {
    if (event.eventType == MouseEventType::Click && event.y == 0 && event.x >= geometry().width - 2 && event.x < geometry().width) {
        // 点击了关闭按钮
        int idx = MessageBoxWindow::question("退出程序？", "关闭控制窗口将导致程序退出！\n无论如何都要退出吗？", std::vector<std::string>{"退出", "取消"});
        if (idx == 0) {
            Tui::instance()->quit();
        }

        return EventProcessResult::Finished;
    } else
        return Window::mouseEvent(event);
}

void ControlWindow::btnShowAllMsgClicked() {
    std::vector<std::vector<std::string>> tableData;
    tableData.push_back(std::vector<std::string>({"学生姓名", "学号", "性别", "专业", "班级", "比赛名称", "赛事级别", "获奖级别"}));
    for (auto& item : AwardRecord::awardList) {
        tableData.push_back(std::vector<std::string>({item.studentName, std::to_string(item.accountNo), to_string(item.sex), item.profession, item.className, item.competitionName, std::string(1, item.competitionGrade), std::string(1, item.awardGrade)}));
    }
    _infoTableWindow.setTableData(tableData);
    _infoTableWindow.setTitle("所有获奖信息");
    _infoTableWindow.show();
}

void ControlWindow::btnInsertElementClicked() {
    _newRecordWindow.show();
}

void ControlWindow::btnDeleteElementClicked() {
    _findRecordWindow.removeEventListener("submit", findRecordWindow_submitEventListenerId);
    findRecordWindow_submitEventListenerId = _findRecordWindow.addEventListener("submit", std::function<void(const AwardRecord::AwardItem&)>([this](const AwardRecord::AwardItem& item) {
                                                                                    int foundCnt = 0;
                                                                                    for (auto it = AwardRecord::awardList.begin(); it != AwardRecord::awardList.end(); ++it) {
                                                                                        if (it->accountNo == item.accountNo && it->competitionName == item.competitionName && it->awardGrade == item.awardGrade) {
                                                                                            foundCnt++;
                                                                                        }
                                                                                    }

                                                                                    if (foundCnt == 0) {
                                                                                        MessageBoxWindow::warning("未找到记录", "未找到匹配的获奖记录！", std::vector<std::string>{"确定"});
                                                                                        return;
                                                                                    }

                                                                                    int result = MessageBoxWindow::question("删除确认", std::format("即将删除 {} 条记录！\n无论如何也要继续吗？", foundCnt), std::vector<std::string>{"删除", "取消"});
                                                                                    if (result == 0) {
                                                                                        int deletedCnt = 0;
                                                                                        for (auto it = AwardRecord::awardList.begin(); it != AwardRecord::awardList.end();) {
                                                                                            if (it->accountNo == item.accountNo && it->competitionName == item.competitionName && it->awardGrade == item.awardGrade) {
                                                                                                it = AwardRecord::awardList.erase(it);
                                                                                                ++deletedCnt;
                                                                                            } else
                                                                                                ++it;
                                                                                        }

                                                                                        MessageBoxWindow::information("删除成功", std::format("已成功删除 {} 条匹配的获奖记录！", deletedCnt), std::vector<std::string>{"确定"});
                                                                                        _findRecordWindow.hide();
                                                                                    }
                                                                                }));

    _findRecordWindow.setFindFilter((FindFilterBy)((int)FindFilterBy::ByAccountNo | (int)FindFilterBy::ByCompetitionName | (int)FindFilterBy::ByAwardLevel));
    _findRecordWindow.setSubmitBtnText("删除");
    _findRecordWindow.setTitle("删除获奖信息");
    _findRecordWindow.show();
}

void ControlWindow::btnDeleteCompetitionClicked() {
    _findRecordWindow.removeEventListener("submit", findRecordWindow_submitEventListenerId);
    findRecordWindow_submitEventListenerId = _findRecordWindow.addEventListener("submit", std::function<void(const AwardRecord::AwardItem&)>([this](const AwardRecord::AwardItem& item) {
                                                                                    int foundCnt = 0;
                                                                                    for (auto it = AwardRecord::awardList.begin(); it != AwardRecord::awardList.end(); ++it) {
                                                                                        if (it->competitionName == item.competitionName) {
                                                                                            foundCnt++;
                                                                                        }
                                                                                    }

                                                                                    if (foundCnt == 0) {
                                                                                        MessageBoxWindow::warning("未找到记录", "未找到匹配的获奖记录！", std::vector<std::string>{"确定"});
                                                                                        return;
                                                                                    }

                                                                                    int result = MessageBoxWindow::question("删除确认", std::format("即将删除 {} 条记录！\n无论如何也要继续吗？", foundCnt), std::vector<std::string>{"删除", "取消"});
                                                                                    if (result == 0) {
                                                                                        int deletedCnt = 0;
                                                                                        for (auto it = AwardRecord::awardList.begin(); it != AwardRecord::awardList.end();) {
                                                                                            if (it->competitionName == item.competitionName) {
                                                                                                it = AwardRecord::awardList.erase(it);
                                                                                                ++deletedCnt;
                                                                                            } else
                                                                                                ++it;
                                                                                        }

                                                                                        MessageBoxWindow::information("删除成功", std::format("已成功删除 {} 条匹配的获奖记录！", deletedCnt), std::vector<std::string>{"确定"});
                                                                                        _findRecordWindow.hide();
                                                                                    }
                                                                                }));

    _findRecordWindow.setFindFilter(FindFilterBy::ByCompetitionName);
    _findRecordWindow.setSubmitBtnText("删除");
    _findRecordWindow.setTitle("根据比赛名称删除获奖信息");
    _findRecordWindow.show();
}

void ControlWindow::btnFindMsgByStuNOClicked() {
    _findRecordWindow.removeEventListener("submit", findRecordWindow_submitEventListenerId);
    findRecordWindow_submitEventListenerId = _findRecordWindow.addEventListener("submit", std::function<void(const AwardRecord::AwardItem&)>([this](const AwardRecord::AwardItem& item) {
                                                                                    std::vector<std::vector<std::string>> tableData;
                                                                                    tableData.push_back(std::vector<std::string>({"学生姓名", "学号", "性别", "专业", "班级", "比赛名称", "赛事级别", "获奖级别"}));
                                                                                    for (auto& record : AwardRecord::awardList) {
                                                                                        if (record.accountNo == item.accountNo) {
                                                                                            tableData.push_back(std::vector<std::string>({record.studentName, std::to_string(record.accountNo), AwardRecord::to_string(record.sex), record.profession, record.className, record.competitionName, std::string(1, record.competitionGrade), std::string(1, record.awardGrade)}));
                                                                                        }
                                                                                    }

                                                                                    if (tableData.size() == 1) {
                                                                                        MessageBoxWindow::warning("未找到记录", "未找到匹配的获奖记录！", std::vector<std::string>{"确定"});
                                                                                        return;
                                                                                    }

                                                                                    _infoTableWindow.setTableData(tableData);
                                                                                    _infoTableWindow.setTitle(std::format("{} 的获奖信息", item.accountNo));
                                                                                    _infoTableWindow.show();
                                                                                    _findRecordWindow.hide();
                                                                                }));
    _findRecordWindow.setFindFilter(FindFilterBy::ByAccountNo);
    _findRecordWindow.setSubmitBtnText("查找");
    _findRecordWindow.setTitle("根据学号查找获奖信息");
    _findRecordWindow.show();
}

void ControlWindow::btnFindMsgByCompetitionClicked() {
    _findRecordWindow.removeEventListener("submit", findRecordWindow_submitEventListenerId);
    findRecordWindow_submitEventListenerId = _findRecordWindow.addEventListener("submit", std::function<void(const AwardRecord::AwardItem&)>([this](const AwardRecord::AwardItem& item) {
                                                                                    std::vector<std::vector<std::string>> tableData;
                                                                                    tableData.push_back(std::vector<std::string>({"学生姓名", "学号", "性别", "专业", "班级", "比赛名称", "赛事级别", "获奖级别"}));
                                                                                    for (auto& record : AwardRecord::awardList) {
                                                                                        if (record.competitionName == item.competitionName) {
                                                                                            tableData.push_back(std::vector<std::string>({record.studentName, std::to_string(record.accountNo), AwardRecord::to_string(record.sex), record.profession, record.className, record.competitionName, std::string(1, record.competitionGrade), std::string(1, record.awardGrade)}));
                                                                                        }
                                                                                    }

                                                                                    if (tableData.size() == 1) {
                                                                                        MessageBoxWindow::warning("未找到记录", "未找到匹配的获奖记录！", std::vector<std::string>{"确定"});
                                                                                        return;
                                                                                    }

                                                                                    _infoTableWindow.setTableData(tableData);
                                                                                    _infoTableWindow.setTitle(std::format("{} 的获奖信息", item.competitionName));
                                                                                    _infoTableWindow.show();
                                                                                    _findRecordWindow.hide();
                                                                                }));
    _findRecordWindow.setFindFilter(FindFilterBy::ByCompetitionName);
    _findRecordWindow.setSubmitBtnText("查找");
    _findRecordWindow.setTitle("根据比赛名称查找获奖信息");
    _findRecordWindow.show();
}

void ControlWindow::btnEmptyListClicked() {
    int result = MessageBoxWindow::question("清空确认", "即将清空所有获奖信息！\n无论如何也要继续吗？", std::vector<std::string>{"清空", "取消"});
    if (result == 0) {
        AwardRecord::awardList.clear();
        MessageBoxWindow::information("清空成功", "已成功清空所有获奖信息！", std::vector<std::string>{"确定"});
    }
}

void ControlWindow::btnEmptyFileDataClicked() {
    int result = MessageBoxWindow::question("清空确认", "即将清空获奖信息文件中的数据！\n无论如何也要继续吗？", std::vector<std::string>{"清空", "取消"});
    if (result == 0) {
        try {
            AwardRecord::ClearFile("data.txt");
            MessageBoxWindow::information("清空成功", "已成功清空获奖信息文件中的数据！", std::vector<std::string>{"确定"});
        } catch (const std::exception& ex) {
            MessageBoxWindow::error("错误", std::format("清空获奖信息文件失败！\n错误信息：{}", ex.what()), std::vector<std::string>{"确定"});
        }
    }
}

void ControlWindow::btnSaveAllMsgToFileClicked() {
    int result = MessageBoxWindow::question("保存确认", "即将覆盖存档文件的内容！\n无论如何也要继续吗？", std::vector<std::string>{"保存", "取消"});
    if (result == 0) {
        try {
            AwardRecord::SaveToFile("data.txt", AwardRecord::awardList);
            MessageBoxWindow::information("保存成功", "已成功把所有获奖信息保存到文件中！", std::vector<std::string>{"确定"});
        } catch (const std::exception& ex) {
            MessageBoxWindow::error("错误", std::format("保存获奖信息到文件失败！\n错误信息：{}", ex.what()), std::vector<std::string>{"确定"});
        }
    }
}

void ControlWindow::btnDelRecordByStudentNoClicked() {
    _findRecordWindow.removeEventListener("submit", findRecordWindow_submitEventListenerId);
    findRecordWindow_submitEventListenerId = _findRecordWindow.addEventListener("submit", std::function<void(const AwardRecord::AwardItem&)>([this](const AwardRecord::AwardItem& item) {
                                                                                    int foundCnt = 0;
                                                                                    for (auto it = AwardRecord::awardList.begin(); it != AwardRecord::awardList.end(); ++it) {
                                                                                        if (it->accountNo == item.accountNo) {
                                                                                            foundCnt++;
                                                                                        }
                                                                                    }

                                                                                    if (foundCnt == 0) {
                                                                                        MessageBoxWindow::warning("未找到记录", "未找到匹配的获奖记录！", std::vector<std::string>{"确定"});
                                                                                        return;
                                                                                    }

                                                                                    int result = MessageBoxWindow::question("删除确认", std::format("即将删除 {} 条记录！\n无论如何也要继续吗？", foundCnt), std::vector<std::string>{"删除", "取消"});
                                                                                    if (result == 0) {
                                                                                        int deletedCnt = 0;
                                                                                        for (auto it = AwardRecord::awardList.begin(); it != AwardRecord::awardList.end();) {
                                                                                            if (it->accountNo == item.accountNo) {
                                                                                                it = AwardRecord::awardList.erase(it);
                                                                                                ++deletedCnt;
                                                                                            } else
                                                                                                ++it;
                                                                                        }

                                                                                        MessageBoxWindow::information("删除成功", std::format("已成功删除 {} 条匹配的获奖记录！", deletedCnt), std::vector<std::string>{"确定"});
                                                                                        _findRecordWindow.hide();
                                                                                    }
                                                                                }));
    _findRecordWindow.setFindFilter(FindFilterBy::ByAccountNo);
    _findRecordWindow.setSubmitBtnText("删除");
    _findRecordWindow.setTitle("根据学生ID删除获奖信息");
    _findRecordWindow.show();
}

void ControlWindow::newRecordWindow_submitEvent(const AwardRecord::AwardItem& newItem) {
    AwardRecord::awardList.push_back(newItem);
    MessageBoxWindow::information("成功", "已成功添加获奖信息！", std::vector<std::string>{"确定"});
}
