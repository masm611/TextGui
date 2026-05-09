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

#pragma once

#include <vector>
#include "widget.hpp"
#include "label.hpp"

#include <string>

class TableWidget : public Widget {
public:
    TableWidget(Widget* parent = nullptr);
    void setSize(int rows, int cols);
    void setCell(int row, int col, const std::string& text);

    virtual Size getMinimumSize() const override;

    virtual void draw() override;

private:
    int _rows;
    int _cols;
    std::vector<std::vector<std::string>> _cells;

    Label renderLabel = Label("", this);

    Size getPrintSize() const;

    void updateLabel();

    bool updatePaused = false;
    void pauseUpdate();
    void resumeUpdate();
};
