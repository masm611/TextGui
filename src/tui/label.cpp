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

    static int oldW = 0;
    static int oldH = 0;

    if (geometry().width != oldW || geometry().height != oldH) {
        oldW = geometry().width;
        oldH = geometry().height;

        if (autoResize_width || autoResize_height)
            autoResizeToFitText();
    }

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

    if (autoResize_width || autoResize_height)
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

    if (autoResize_height || autoResize_width)
        autoResizeToFitText();
}

void Label::setAutoResize(bool widthGrow, bool heightGrow) {
    autoResize_width = widthGrow;
    autoResize_height = heightGrow;

    if (autoResize_height || autoResize_width)
        autoResizeToFitText();
}

void Label::alignTextToRowCenter(bool enable) {
    alignTextToRowCenterEnabled = enable;
}

void Label::autoResizeToFitText() {
    Size newSize;
    if (autoResize_width) {
        // 寻找最大行宽
        int maxLineWidth = 0;
        size_t start = 0;
        for (size_t i = 0; i < text.size(); ++i)
            if (text[i] == '\n') {
                int lineWidth = getPrintWidth(std::string_view(text.data() + start, i - start));
                maxLineWidth = std::max(maxLineWidth, lineWidth);
                start = i + 1;
            }

        newSize.width = maxLineWidth;
    } else {
        newSize.width = geometry().width;
    }

    if (autoResize_height) {
        if (autoResize_width) {
            // 计算行数
            int lineCount = 0;
            size_t start = 0;
            for (size_t i = 0; i < text.size(); ++i)
                if (text[i] == '\n') {
                    ++lineCount;
                    start = i + 1;
                }
            ++lineCount;

            newSize.height = lineCount;
        } else {
            // 计算显示完所需行数
            int lineWidth = newSize.width;

            int curX = 0;
            int curY = 0;

            std::vector<Utf8Char> u8c = getUtf8Chars(text);
            for (int i = 0; i < text.size(); ++i) {
                if (u8c[i][0] == '\n') {
                    curX = 0;
                    ++curY;
                    continue;
                }

                uint8_t width = getPrintWidth(u8c[i]);
                if (curX + width > lineWidth) {
                    curX = width;
                    ++curY;
                } else {
                    curX += width;
                }
            }

            newSize.height = curY + 1;
        }
    }

    Rect geometry = this->geometry();
    geometry.width = newSize.width;
    geometry.height = newSize.height;
    setGeometry(geometry);
}
