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

#include "tableWidget.hpp"

#include <sstream>

TableWidget::TableWidget(Widget* parent) : Widget(parent) {
    renderLabel.setWordWrap(false);
    renderLabel.show();
}

void TableWidget::setSize(int rows, int cols) {
    _rows = rows;
    _cols = cols;
    _cells = std::vector<std::vector<std::string>>(rows, std::vector<std::string>(cols));
}

void TableWidget::setCell(int row, int col, const std::string& text) {
    if (row < 0 || row >= _rows || col < 0 || col >= _cols)
        throw std::out_of_range("TableWidget::setCell: row or col out of range");

    _cells[row][col] = text;

    if (!updatePaused)
        updateLabel();
}

Size TableWidget::getMinimumSize() const {
    return getPrintSize();
}

void TableWidget::draw() {
    Widget::draw();
}

Size TableWidget::getPrintSize() const {
    // 计算每列需要多少宽度
    std::vector<uint32_t> maxLen(_cells[0].size(), 0);
    for (const auto& row : _cells) {
        for (uint64_t i = 0; i < row.size(); ++i)
            maxLen[i] = std::max(maxLen[i], getPrintWidth(row[i]) + 2);
    }

    int width = 2;
    for (const auto& len : maxLen)
        width += len;
    return Size{width, (int)_cells.size() + 1};
}

void TableWidget::updateLabel() {
    std::stringstream ss;

    // 计算每列需要多少宽度
    std::vector<uint32_t> maxLen(_cells[0].size(), 0);
    for (const auto& row : _cells) {
        for (uint64_t i = 0; i < row.size(); ++i)
            maxLen[i] = std::max(maxLen[i], getPrintWidth(row[i]) + 2);
    }

    // 逐行打印
    for (const auto& row : _cells) {
        // 打印分割线
        for (uint64_t i = 0; i < row.size(); ++i) {
            ss << '+';
            for (uint32_t j = 0; j < maxLen[i]; ++j)
                ss << '-';
        }
        ss << '+' << std::endl;

        // 打印数据项
        for (uint64_t i = 0; i < row.size(); ++i) {
            int a = getPrintWidth(row[i]);
            ss << "| " << row[i];
            for (uint32_t j = 0; j < maxLen[i] - getPrintWidth(row[i]) - 1; ++j)
                ss << ' ';
        }
        ss << '|' << std::endl;
    }

    // 打印分割线
    for (uint64_t i = 0; i < _cells[0].size(); ++i) {
        ss << '+';
        for (uint32_t j = 0; j < maxLen[i]; ++j)
            ss << '-';
    }
    ss << '+' << std::endl;

    renderLabel.setText(ss.str());
    Size labelSize = renderLabel.getMinimumSize();
    renderLabel.setGeometry(Rect(0, 0, labelSize.width, labelSize.height));
    setGeometry(Rect(geometry().x, geometry().y, labelSize.width, labelSize.height));
}

void TableWidget::pauseUpdate() {
    updatePaused = true;
}

void TableWidget::resumeUpdate() {
    updatePaused = false;
    updateLabel();
}
