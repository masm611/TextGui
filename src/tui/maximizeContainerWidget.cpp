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

#include "maximizeContainerWidget.hpp"

#include "painter.hpp"

MaximizeContainerWidget::MaximizeContainerWidget(Widget* parent) : Widget(parent) {
}

void MaximizeContainerWidget::draw() {
    Rect drawableArea = getParent()->getChildDrawableArea(this);
    Rect targetGeometry = drawableArea;

    targetGeometry.x = 0;
    targetGeometry.y = 0;

    if (!maximazeWidth)
        targetGeometry.width = geometry().width;

    if (!maximazeHeight)
        targetGeometry.height = geometry().height;

    setGeometry(targetGeometry);
    getChildren()[0]->setGeometry(targetGeometry);

    Widget::draw();
}

void MaximizeContainerWidget::setMaximize(bool width, bool height) {
    maximazeWidth = width;
    maximazeHeight = height;
}

void MaximizeContainerWidget::setWidth(int width) {
    setGeometry(Rect(geometry().x, geometry().y, width, geometry().height));
}

void MaximizeContainerWidget::setHeight(int height) {
    setGeometry(Rect(geometry().x, geometry().y, geometry().width, height));
}
