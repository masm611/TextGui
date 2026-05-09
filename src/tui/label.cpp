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

#include "label.hpp"

#include "painter.hpp"
#include "utf8Utils.hpp"

void Label::draw() {
    Widget::draw();

    Painter painter(this);

    if (wordWrap) {
        int beginX = 0, beginY = 0;

        if (alignTextToRowCenterEnabled) {
            int w = getPrintWidth(text);
            int availableWidth = geometry().width;
            if (w < availableWidth)
                beginX = (availableWidth - w) / 2;
        }

        if (!textBgColorEnabled)
            painter.drawText(Rect(beginX, 0, geometry().width - beginX, geometry().height), text, fgColor);
        else
            painter.drawText(Rect(beginX, 0, geometry().width - beginX, geometry().height), text, fgColor, textBgColor);
    } else {
        int beginX = 0, beginY = 0;

        if (alignTextToRowCenterEnabled) {
            auto [w, h] = getMultiRowPrintSize(text);
            int availableWidth = geometry().width;
            if (w < availableWidth)
                beginX = (availableWidth - w) / 2;
        }

        // 按照\n分出每一行
        std::vector<std::string_view> lines;
        size_t start = 0;
        for (size_t i = 0; i < text.size(); ++i)
            if (text[i] == '\n') {
                lines.push_back(std::string_view(text.data() + start, i - start));
                start = i + 1;
            }
        lines.push_back(std::string_view(text.data() + start, text.size() - start));

        for (int lineIndex = 0; lineIndex < lines.size(); ++lineIndex) {
            if (!textBgColorEnabled)
                painter.drawText(Rect(beginX, lineIndex, geometry().width - beginX, 1), limitStrPrintWidth(lines[lineIndex], geometry().width - beginX), fgColor);
            else
                painter.drawText(Rect(beginX, lineIndex, geometry().width, 1), limitStrPrintWidth(lines[lineIndex], geometry().width), fgColor, textBgColor);
        }
    }
}

void Label::setText(const std::string& newText) {
    text = newText;

    if (autoResize)
        autoResizeToFitText();
}

void Label::setTextColor(const Color& color) {
    fgColor = color;
}

void Label::setTextBackgroundColor(const Color& color) {
    textBgColor = color;
    textBgColorEnabled = true;
}

void Label::disableTextBackgroundColor() {
    textBgColorEnabled = false;
}

Size Label::getMinimumSize() const {
    int maxLineWidth = 0;
    int lineCount = 0;
    size_t start = 0;
    for (size_t i = 0; i < text.size(); ++i)
        if (text[i] == '\n') {
            int lineWidth = getPrintWidth(std::string_view(text.data() + start, i - start));
            maxLineWidth = std::max(maxLineWidth, lineWidth);
            start = i + 1;
            ++lineCount;
        }
    int lineWidth = getPrintWidth(std::string_view(text.data() + start, text.size() - start));
    maxLineWidth = std::max(maxLineWidth, lineWidth);
    ++lineCount;

    return Size{maxLineWidth, lineCount};
}

void Label::setWordWrap(bool enable) {
    wordWrap = enable;

    if (autoResize)
        autoResizeToFitText();
}

void Label::setAutoResize(bool enable) {
    autoResize = enable;

    if (autoResize)
        autoResizeToFitText();
}

void Label::alignTextToRowCenter(bool enable) {
    alignTextToRowCenterEnabled = enable;
}

void Label::autoResizeToFitText() {
    Size minSize = getMinimumSize();
    Rect geometry = this->geometry();
    geometry.width = minSize.width;
    geometry.height = minSize.height;
    setGeometry(geometry);
}
