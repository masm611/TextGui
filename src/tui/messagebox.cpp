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

#include "messagebox.hpp"

#include "window.hpp"
#include "tui.hpp"
#include "maximizeContainerWidget.hpp"
#include "verticalLayoutWidget.hpp"
#include "horizontalLayoutWidget.hpp"
#include "label.hpp"
#include "marginContainerWidget.hpp"

int MessageBoxWindow::exec(const std::string& title, const std::string& message, const std::vector<std::string>& buttons, CanvasWidget* icon) {
    // 计算宽高
    int titleWidth = getPrintWidth(title) + 6;
    int row0Width = 0, row0Height = 0;
    int row1Width = 0, row1Height = 0;

    auto [messageWidth, messageHeight] = getMultiRowPrintSize(message);
    row0Width = (icon ? icon->canvasWidth() + 2 : 0) + messageWidth;
    row0Height = std::max<int>(icon ? icon->canvasHeight() : 0, messageHeight);

    row1Height = (buttons.size() > 0) ? 3 : 0;
    for (const auto& btn : buttons) {
        int btnWidth = getPrintWidth(btn) + 4;
        row1Width += btnWidth + 1;
    }

    int windowWidth = std::max({titleWidth, row0Width + 4, row1Width + 4});
    int windowHeight = row0Height + row1Height + 4;

    // 构建窗口
    Tui* tui = Tui::instance();
    if (tui == nullptr)
        return -1;

    auto screenWidth = tui->width();
    auto screenHeight = tui->height();

    Window msgBox;
    msgBox.setTitle(title);
    msgBox.setGeometry(Rect((screenWidth - windowWidth) / 2, (screenHeight - windowHeight) / 2, windowWidth, windowHeight));
    msgBox.setBackgroundColor(Color(70, 70, 70));
    msgBox.show();

    MaximizeContainerWidget mc(&msgBox);
    mc.setMaximize(true, true);
    mc.show();

    VerticalLayoutWidget vc(&mc);
    vc.setHorizontalSizePolicy(SizePolicy::Expanding);
    vc.setVerticalSizePolicy(SizePolicy::Expanding);
    vc.show();

    HorizontalLayoutWidget row0(&vc);
    row0.setHorizontalSizePolicy(SizePolicy::Expanding);
    row0.setVerticalSizePolicy(SizePolicy::Expanding);
    row0.show();

    Widget leftMargin;
    if (icon != nullptr) {
        leftMargin.setParent(&row0);
        leftMargin.setHorizontalSizePolicy(SizePolicy::Shrink);
        leftMargin.setVerticalSizePolicy(SizePolicy::Shrink);
        leftMargin.setMinimumSize(Size(2, 0));
        leftMargin.show();

        icon->setHorizontalSizePolicy(SizePolicy::Shrink);
        icon->setVerticalSizePolicy(SizePolicy::Shrink);
        icon->setParent(&row0);
        row0.addChild(icon);
        icon->show();
    }

    Label messageLabel(message, &row0);
    messageLabel.setTextColor(Color(255, 255, 255));
    messageLabel.setHorizontalSizePolicy(SizePolicy::Expanding);
    messageLabel.setVerticalSizePolicy(SizePolicy::Shrink);
    messageLabel.setWordWrap(false);
    messageLabel.alignTextToRowCenter(true);
    messageLabel.show();

    HorizontalLayoutWidget row1(&vc);
    row1.setHorizontalSizePolicy(SizePolicy::Expanding);
    row1.setVerticalSizePolicy(SizePolicy::Shrink);
    row1.show();

    std::vector<Button*> buttonWidgets;
    std::vector<Widget*> spacerWidgets;

    Widget* frontSpacer = new Widget(&row1);
    frontSpacer->setHorizontalSizePolicy(SizePolicy::Expanding);
    frontSpacer->setVerticalSizePolicy(SizePolicy::Shrink);
    frontSpacer->show();
    spacerWidgets.push_back(frontSpacer);

    for (int i = 0; i < buttons.size(); ++i) {
        const auto& btnStr = buttons[i];

        Button* btnWidget = new Button(btnStr, &row1);
        btnWidget->setHorizontalSizePolicy(SizePolicy::Shrink);
        btnWidget->setVerticalSizePolicy(SizePolicy::Shrink);
        btnWidget->setMinimumSize(Size(-1, 1));
        btnWidget->show();
        buttonWidgets.push_back(btnWidget);

        Widget* spacer = new Widget(&row1);
        spacer->setHorizontalSizePolicy(SizePolicy::Expanding);
        spacer->setVerticalSizePolicy(SizePolicy::Shrink);
        spacer->show();
        spacerWidgets.push_back(spacer);
    }

    Widget* bottomSpacer = new Widget(&vc);
    bottomSpacer->setGeometry(Rect(0, 0, 1, 1));
    bottomSpacer->setHorizontalSizePolicy(SizePolicy::Expanding);
    bottomSpacer->setVerticalSizePolicy(SizePolicy::Fixed);
    bottomSpacer->show();
    spacerWidgets.push_back(bottomSpacer);

    // 绑定事件
    int retVal = 0;
    msgBox.addEventListener("windowClose", std::function<void()>([&]() {
                                retVal = -1;
                            }));
    for (int i = 0; i < buttonWidgets.size(); ++i)
        buttonWidgets[i]->addEventListener("buttonClicked", std::function<void()>([&, i]() {
                                               retVal = i + 1;
                                           }));

    // eventLoop
    while (retVal == 0 && tui->isEventLoopShouldQuit() == false) {
        tui->processEvents();
    }

    return retVal - 1;
}

int MessageBoxWindow::question(const std::string& title, const std::string& message, const std::vector<std::string>& buttons) {
    CanvasWidget icon;
    icon.setSize(14, 7);

    Pixel pixel;
    pixel.character = {' ', 0, 0, 0};
    pixel.foregroundColor = Color(255, 201, 14);
    pixel.backgroundColor = Color(255, 201, 14);

    std::vector<std::tuple<short, short>> questionMarkPixels({{1, 1}, {2, 0}, {3, 0}, {4, 0}, {5, 1}, {5, 2}, {4, 3}, {3, 3}, {3, 4}, {3, 6}});

    for (const auto& [x, y] : questionMarkPixels) {
        icon.setPixel(x * 2, y, pixel);
        icon.setPixel(x * 2 + 1, y, pixel);
    }

    return exec(title, message, buttons, &icon);
}

int MessageBoxWindow::warning(const std::string& title, const std::string& message, const std::vector<std::string>& buttons) {
    CanvasWidget icon;
    icon.setSize(14, 7);

    Pixel pixel;
    pixel.character = {' ', 0, 0, 0};
    pixel.foregroundColor = Color(255, 127, 39);
    pixel.backgroundColor = Color(255, 127, 39);

    std::vector<std::tuple<short, short>> questionMarkPixels({{3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 6}});

    for (const auto& [x, y] : questionMarkPixels) {
        icon.setPixel(x * 2, y, pixel);
        icon.setPixel(x * 2 + 1, y, pixel);
    }

    return exec(title, message, buttons, &icon);
}

int MessageBoxWindow::information(const std::string& title, const std::string& message, const std::vector<std::string>& buttons) {
    CanvasWidget icon;
    icon.setSize(14, 7);

    Pixel pixel;
    pixel.character = {' ', 0, 0, 0};
    pixel.foregroundColor = Color(0, 162, 232);
    pixel.backgroundColor = Color(0, 162, 232);

    std::vector<std::tuple<short, short>> questionMarkPixels({{3, 0}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, {3, 6}});

    for (const auto& [x, y] : questionMarkPixels) {
        icon.setPixel(x * 2, y, pixel);
        icon.setPixel(x * 2 + 1, y, pixel);
    }

    return exec(title, message, buttons, &icon);
}

int MessageBoxWindow::error(const std::string& title, const std::string& message, const std::vector<std::string>& buttons) {
    CanvasWidget icon;
    icon.setSize(14, 7);

    Pixel pixel;
    pixel.character = {' ', 0, 0, 0};
    pixel.foregroundColor = Color(237, 28, 36);
    pixel.backgroundColor = Color(237, 28, 36);

    std::vector<std::tuple<short, short>> questionMarkPixels({{0, 0}, {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6}, {6, 0}, {5, 1}, {4, 2}, {3, 3}, {2, 4}, {1, 5}, {0, 6}});

    for (const auto& [x, y] : questionMarkPixels) {
        icon.setPixel(x * 2, y, pixel);
        icon.setPixel(x * 2 + 1, y, pixel);
    }

    return exec(title, message, buttons, &icon);
}
