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

#include "marginContainerWidget.hpp"

MarginContainerWidget::MarginContainerWidget(Widget* parent) : Widget(parent) {
}

void MarginContainerWidget::draw() {
    if (!isVisible())
        return;

    Rect geo = geometry();
    Rect childGeo(
        0,
        0,
        geo.width - _leftMargin - _rightMargin,
        geo.height - _topMargin - _bottomMargin);

    if (getChildren().size() > 0)
        getChildren()[0]->setGeometry(childGeo);

    Widget::draw();
}

Size MarginContainerWidget::getMinimumSize() const {
    if (getChildren().size() == 0)
        return Size(_leftMargin + _rightMargin, _topMargin + _bottomMargin);

    Size childMinSize = getChildren()[0]->getMinimumSize();
    return Size(
        childMinSize.width + _leftMargin + _rightMargin,
        childMinSize.height + _topMargin + _bottomMargin);
}

Rect MarginContainerWidget::getChildDrawableArea(const Widget*) const {
    Rect geo = geometry();
    return Rect(
        _leftMargin,
        _topMargin,
        geo.width - _leftMargin - _rightMargin,
        geo.height - _topMargin - _bottomMargin);
}

void MarginContainerWidget::setMargin(short top, short right, short bottom, short left) {
    _topMargin = top;
    _rightMargin = right;
    _bottomMargin = bottom;
    _leftMargin = left;
}