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

#include "verticalLayoutWidget.hpp"

#include <algorithm>

VerticalLayoutWidget::VerticalLayoutWidget(Widget* parent) : Widget(parent) {
}

void VerticalLayoutWidget::draw() {
    updateChildWidgetGeometry();

    Widget::draw();
}

Size VerticalLayoutWidget::getMinimumSize() const {
    int minWidth = 0;
    int minHeight = 0;

    for (auto child : getChildren()) {
        Size childMinSize = child->getMinimumSize();
        if (child->getVerticalSizePolicy() == SizePolicy::Fixed)
            minHeight += child->geometry().height;
        else
            minHeight += childMinSize.height;
        minWidth = std::max(minWidth, childMinSize.width);
    }
    return Size(minWidth, minHeight);
}

void VerticalLayoutWidget::updateChildWidgetGeometry() {
    std::vector<int> childHeights(getChildren().size(), 0);
    int expandingCount = 0;
    const auto children = getChildren();

    if (children.size() == 0)
        return;

    // 先分配shrink和fixed的高度
    int totalFixedHeight = 0;  // 包括shrink和fixed的高度
    for (int i = 0; i < children.size(); ++i) {
        const auto child = children[i];
        if (child->getVerticalSizePolicy() == SizePolicy::Fixed) {
            childHeights[i] = child->geometry().height;
            totalFixedHeight += child->geometry().height;
        } else if (child->getVerticalSizePolicy() == SizePolicy::Shrink) {
            childHeights[i] = child->getMinimumSize().height;
            totalFixedHeight += child->getMinimumSize().height;
        } else if (child->getVerticalSizePolicy() == SizePolicy::Expanding) {
            ++expandingCount;
            int minHeight = child->getMinimumSize().height;

            childHeights[i] = minHeight;  // 暂时分配最小高度
        }
    }

    // 计算剩余高度
    int remainingHeight = geometry().height - totalFixedHeight;
    if (remainingHeight > 0 && expandingCount > 0) {
        // 将高度平均分配给expanding的子组件
        // 执行整数均分
        int base = remainingHeight / expandingCount;
        int rem = remainingHeight % expandingCount;

        int basePlusOneCnt = 0;
        for (int i = 0; i < children.size(); ++i) {
            const auto child = children[i];
            if (child->getVerticalSizePolicy() == SizePolicy::Expanding) {
                childHeights[i] = base + (basePlusOneCnt < rem ? 1 : 0);
                if (basePlusOneCnt < rem)
                    ++basePlusOneCnt;
            }
        }
    }

    // 设置子组件的geometry
    int currentY = 0;
    for (int i = 0; i < children.size(); ++i) {
        const auto child = children[i];

        Rect childGeometry;

        // 水平轴
        SizePolicy hSizePolicy = child->getHorizontalSizePolicy();
        if (hSizePolicy == SizePolicy::Fixed)
            childGeometry.width = child->geometry().width;
        else if (hSizePolicy == SizePolicy::Shrink)
            childGeometry.width = child->getMinimumSize().width;
        else if (hSizePolicy == SizePolicy::Expanding)
            childGeometry.width = this->geometry().width;

        if (hSizePolicy == SizePolicy::Fixed || hSizePolicy == SizePolicy::Shrink)
            childGeometry.x = (geometry().width - childGeometry.width) / 2;
        else
            childGeometry.x = 0;

        // 垂直轴
        childGeometry.y = currentY;
        childGeometry.height = childHeights[i];
        currentY += childGeometry.height;

        child->setGeometry(childGeometry);
    }
}
