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

#include "canvas.hpp"
#include "widget.hpp"
#include "utf8Utils.hpp"

#include <span>

class Painter {
public:
    Painter(Canvas* canvas, Rect& drawableArea);
    Painter(Widget* widget);
    ~Painter();

    void setPixel(short x, short y, Pixel pixel);

    void fill(Rect rect, const Pixel& pixel);
    void fill(Rect rect, const Color& bgColor);

    void drawText(Rect rect, const std::string_view& text, const Color& fgColor, const Color& bgColor);
    void drawText(Rect rect, const std::string_view& text, const Color& fgColor);
    void drawText(Rect rect, const std::vector<Utf8Char>& u8cArray, const Color& fgColor, const Color& bgColor);
    void drawText(Rect rect, const std::vector<Utf8Char>& u8cArray, const Color& fgColor);
    void drawText(Rect rect, const std::span<const Utf8Char>& u8cArray, const Color& fgColor, const Color& bgColor);
    void drawText(Rect rect, const std::span<const Utf8Char>& u8cArray, const Color& fgColor);

    void reverseFgBgColor(short x, short y);

    Painter subPainter(Rect subArea);

    static Rect getRealDrawableArea(Widget* widget);

private:
    Rect drawableArea;
    Rect realDrawableArea;
    Canvas* canvas;
    Widget* widget;
};