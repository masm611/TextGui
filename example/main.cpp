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

// 竞赛获奖信息管理系统
#include <vector>
#include <list>
#include "tui/tui.hpp"
#include "tui/utf8Utils.hpp"

#include <iostream>
#include <fstream>
#include <cstdint>

#include "tui/label.hpp"
#include "tui/window.hpp"
#include "tui/button.hpp"
#include "tui/terminalWindow.hpp"
#include "tui/lineTextInput.hpp"
#include "tui/scrollWidget.hpp"
#include "tui/horizontalLayoutWidget.hpp"
#include "tui/verticalLayoutWidget.hpp"
#include "tui/maximizeContainerWidget.hpp"
#include "tui/marginContainerWidget.hpp"
#include "controlWindow.hpp"
#include "tui/tableWidget.hpp"

int main() {
    Tui tui;
    tui.init();

    ControlWindow controlWindow;

    tui.loop();
    return 0;
}

// void testTui() {
//     auto u8cArray = getUtf8Chars("🗗");

//     Tui tui;
//     tui.init();

//     Window downWindow;
//     downWindow.setGeometry(Rect(0, 0, 50, 20));
//     downWindow.setBackgroundColor(Color(0, 127, 255));
//     downWindow.setTitle("堆叠测试窗口");
//     downWindow.show();

//     MaximizeContainerWidget mc(&downWindow);
//     mc.show();

//     VerticalLayoutWidget _vc0(&mc);
//     _vc0.show();

//     HorizontalLayoutWidget _hc0(&_vc0);
//     _hc0.setHorizontalSizePolicy(SizePolicy::Expanding);
//     _hc0.setVerticalSizePolicy(SizePolicy::Expanding);
//     _hc0.show();

//     Label usernameLabel("用户名", &_hc0);
//     usernameLabel.setTextColor(Color(255, 255, 255));
//     usernameLabel.setHorizontalSizePolicy(SizePolicy::Shrink);
//     usernameLabel.setVerticalSizePolicy(SizePolicy::Shrink);
//     usernameLabel.show();

//     LineTextInput usernameInput(&_hc0);
//     usernameInput.setHorizontalSizePolicy(SizePolicy::Expanding);
//     usernameInput.setVerticalSizePolicy(SizePolicy::Shrink);
//     usernameInput.setBackgroundColor(Color(255, 255, 255));
//     usernameInput.show();

//     Button btn("点我", &downWindow);
//     btn.setGeometry(Rect(10, 1, 10, 3));
//     btn.addEventListener("buttonClicked", std::function<void(void)>([&]() {
//                              static int clickCount = 0;
//                              clickCount++;

//                              btn.setText("点了" + std::to_string(clickCount) + "下");
//                          }));

//     Window parentWindow;
//     parentWindow.setGeometry(Rect(30, 7, 50, 20));
//     parentWindow.setBackgroundColor(Color(255, 127, 39));
//     parentWindow.setTitle("测试窗口");

//     Window childWindow(&parentWindow);
//     childWindow.setGeometry(Rect(5, 5, 30, 10));
//     childWindow.setBackgroundColor(Color(0, 255, 0));
//     childWindow.setTitle("子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口子窗口");

//     Label label("你好，中华人民共和国成立于1949年，asdfasdlkfjasdklfjaklsdgjiasdfjglaksdjgaisdgjij", &parentWindow);
//     label.setGeometry(Rect(10, 1, 19, 5));
//     label.setTextColor(Color(0, 0, 0));
//     label.setBackgroundColor(Color(255, 255, 0));

//     Window scrollableWindow;
//     scrollableWindow.setGeometry(Rect(0, 0, 50, 20));
//     scrollableWindow.setBackgroundColor(Color(255, 127, 39));
//     scrollableWindow.setTitle("滚动窗口");
//     scrollableWindow.show();

//     MaximizeContainerWidget mc0(&scrollableWindow);
//     mc0.show();

//     ScrollWidget scrollWidget(&mc0);
//     scrollWidget.setGeometry(Rect(0, 0, 20, 15));
//     scrollWidget.setScrollable(true, true);
//     scrollWidget.show();

//     Label scrollContentLabel("这是一个滚动窗口的内容。这个内容非常长，需要滚动才能看到全部。这是一个滚动窗口的内容。这个内容非常长，需要滚动才能看到全部。这是一个滚动窗口的内容\n。这个内容非常长，需要滚动才能看到全部。这是一个滚动窗口的内容。这个内容非常长，需要滚动才能看到全部。这是一个滚动窗口的内容。这个内容非常长，需要滚动才能看到全部。", &scrollWidget);
//     scrollContentLabel.setGeometry(Rect(0, 0, 50, 20));
//     scrollContentLabel.setTextColor(Color(0, 0, 0));
//     scrollContentLabel.setBackgroundColor(Color(255, 255, 0));
//     scrollContentLabel.setWordWrap(false);
//     scrollContentLabel.setAutoResize(true);
//     scrollContentLabel.show();

//     Label endLabel("", &scrollWidget);
//     endLabel.setGeometry(Rect(0, 20, 49, 1));
//     endLabel.setBackgroundColor(Color(255, 255, 255));
//     endLabel.show();

//     Label endLabel2("", &scrollWidget);
//     endLabel2.setGeometry(Rect(49, 20, 1, 1));
//     endLabel2.setBackgroundColor(Color(0, 255, 0));
//     endLabel2.show();

//     Window layoutWindow;
//     layoutWindow.setGeometry(Rect(30, 0, 50, 20));
//     layoutWindow.setBackgroundColor(Color(30, 30, 30));
//     layoutWindow.setTitle("布局窗口");
//     layoutWindow.show();

//     HorizontalLayoutWidget horizontalLayout(&layoutWindow);
//     horizontalLayout.setGeometry(Rect(0, 0, 50, 19));
//     horizontalLayout.show();

//     Label layoutLabel1("标签1", &horizontalLayout);
//     layoutLabel1.setTextColor(Color(255, 255, 255));
//     layoutLabel1.setHorizontalSizePolicy(SizePolicy::Expanding);
//     layoutLabel1.setVerticalSizePolicy(SizePolicy::Shrink);
//     layoutLabel1.show();

//     Label layoutLabel2("标签2", &horizontalLayout);
//     layoutLabel2.setTextColor(Color(255, 255, 255));
//     layoutLabel2.setHorizontalSizePolicy(SizePolicy::Shrink);
//     layoutLabel2.setVerticalSizePolicy(SizePolicy::Shrink);
//     layoutLabel2.show();

//     Window verticalLayoutWindow;
//     verticalLayoutWindow.setGeometry(Rect(80, 0, 50, 20));
//     verticalLayoutWindow.setBackgroundColor(Color(30, 30, 30));
//     verticalLayoutWindow.setTitle("垂直布局窗口");
//     verticalLayoutWindow.show();

//     MaximizeContainerWidget maximizeContainer(&verticalLayoutWindow);
//     maximizeContainer.show();

//     VerticalLayoutWidget verticalLayout(&maximizeContainer);
//     verticalLayout.show();

//     Label vLayoutLabel1("标签1", &verticalLayout);
//     vLayoutLabel1.setTextColor(Color(255, 255, 255));
//     vLayoutLabel1.setHorizontalSizePolicy(SizePolicy::Shrink);
//     vLayoutLabel1.setVerticalSizePolicy(SizePolicy::Expanding);
//     vLayoutLabel1.show();

//     Label vLayoutLabel2("标签2", &verticalLayout);
//     vLayoutLabel2.setTextColor(Color(255, 255, 255));
//     vLayoutLabel2.setHorizontalSizePolicy(SizePolicy::Shrink);
//     vLayoutLabel2.setVerticalSizePolicy(SizePolicy::Shrink);
//     vLayoutLabel2.show();

//     Button vLayoutBtn("点我", &verticalLayout);
//     vLayoutBtn.setBackgroundColor(Color(255, 255, 255));
//     vLayoutBtn.setHorizontalSizePolicy(SizePolicy::Expanding);
//     vLayoutBtn.setVerticalSizePolicy(SizePolicy::Expanding);
//     vLayoutBtn.show();
//     // label.setTextBackgroundColor(Color(255, 255, 0));

//     // TerminalWindow terminalWindow;
//     // terminalWindow.setGeometry(Rect(0, 0, 30, 10));
//     // terminalWindow.setTitle("终端窗口");

//     tui.loop();
// }
