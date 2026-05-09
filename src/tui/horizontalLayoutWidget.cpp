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

#include "horizontalLayoutWidget.hpp"

HorizontalLayoutWidget::HorizontalLayoutWidget(Widget* parent) : Widget(parent) {
}

void HorizontalLayoutWidget::draw() {
    updateChildWidgetGeometry();

    Widget::draw();
}

Size HorizontalLayoutWidget::getMinimumSize() const {
    int minWidth = 0;
    int minHeight = 0;

    for (auto child : getChildren()) {
        Size childMinSize = child->getMinimumSize();
        if (child->getHorizontalSizePolicy() == SizePolicy::Fixed)
            minWidth += child->geometry().width;
        else
            minWidth += childMinSize.width;
        minHeight = std::max(minHeight, childMinSize.height);
    }
    return Size(minWidth, minHeight);
}

void HorizontalLayoutWidget::updateChildWidgetGeometry() {
    std::vector<int> childWidths(getChildren().size(), 0);
    int expandingCount = 0;
    const auto children = getChildren();

    if (children.size() == 0)
        return;

    // 先分配shrink和fixed的宽度
    int totalFixedWidth = 0;  // 包括shrink和fixed的宽度
    for (int i = 0; i < children.size(); ++i) {
        const auto child = children[i];
        if (child->getHorizontalSizePolicy() == SizePolicy::Fixed) {
            childWidths[i] = child->geometry().width;
            totalFixedWidth += child->geometry().width;
        } else if (child->getHorizontalSizePolicy() == SizePolicy::Shrink) {
            childWidths[i] = child->getMinimumSize().width;
            totalFixedWidth += child->getMinimumSize().width;
        } else if (child->getHorizontalSizePolicy() == SizePolicy::Expanding) {
            ++expandingCount;
            int minWidth = child->getMinimumSize().width;

            childWidths[i] = minWidth;  // 暂时分配最小宽度
        }
    }

    // 计算剩余宽度
    int remainingWidth = geometry().width - totalFixedWidth;
    if (remainingWidth > 0 && expandingCount > 0) {
        // 将宽度平均分配给expanding的子组件
        // 执行整数均分
        int base = remainingWidth / expandingCount;
        int rem = remainingWidth % expandingCount;

        int basePlusOneCnt = 0;
        for (int i = 0; i < children.size(); ++i) {
            const auto child = children[i];
            if (child->getHorizontalSizePolicy() == SizePolicy::Expanding) {
                childWidths[i] = base + (basePlusOneCnt < rem ? 1 : 0);
                if (basePlusOneCnt < rem)
                    ++basePlusOneCnt;
            }
        }
    }

    // 设置子组件的geometry
    int currentX = 0;
    for (int i = 0; i < children.size(); ++i) {
        const auto child = children[i];

        Rect childGeometry;

        // 垂直轴
        SizePolicy vSizePolicy = child->getVerticalSizePolicy();
        if (vSizePolicy == SizePolicy::Fixed)
            childGeometry.height = child->geometry().height;
        else if (vSizePolicy == SizePolicy::Shrink)
            childGeometry.height = child->getMinimumSize().height;
        else if (vSizePolicy == SizePolicy::Expanding)
            childGeometry.height = this->geometry().height;

        if (vSizePolicy == SizePolicy::Fixed || vSizePolicy == SizePolicy::Shrink)
            childGeometry.y = (geometry().height - childGeometry.height) / 2;
        else
            childGeometry.y = 0;

        // 水平轴
        childGeometry.x = currentX;
        childGeometry.width = childWidths[i];
        currentX += childGeometry.width;

        child->setGeometry(childGeometry);
    }
}
