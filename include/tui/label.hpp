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

#include "widget.hpp"

class Label : public Widget {
public:
    Label(const std::string& text, Widget* parent = nullptr) : Widget(parent), text(text) {}

    void draw() override;

    std::string_view getText() const { return text; }
    void setText(const std::string& newText);

    void setTextColor(const Color& color);
    void setTextBackgroundColor(const Color& color);
    void disableTextBackgroundColor();

    virtual Size getMinimumSize() const override;

    void setWordWrap(bool enable);
    void setAutoResize(bool widthGrow, bool heightGrow);

    void alignTextToRowCenter(bool enable);

private:
    void autoResizeToFitText();

    Color textBgColor = Color(0, 0, 0);
    bool textBgColorEnabled = false;

    Color fgColor = Color(255, 255, 255);
    std::string text;

    bool wordWrap = true;
    bool autoResize_width = false;
    bool autoResize_height = false;
    bool alignTextToRowCenterEnabled = false;
};