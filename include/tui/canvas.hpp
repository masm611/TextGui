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

#include "pixel.hpp"
#include "color.hpp"
#include "rect.hpp"

#include <string>
#include <tuple>

class Canvas {
public:
    Canvas(short width, short height);
    ~Canvas();

    std::tuple<short, short> getSize() const;

    void setPixel(short x, short y, const Pixel& pixel);
    const Pixel& getPixel(short x, short y) const;

    void fill(Rect rect, const Pixel& pixel);
    void fill(Rect rect, const Color& bgColor);

    void drawText(Rect rect, const std::string& text, const Color& fgColor, const Color& bgColor);
    void drawText(Rect rect, const std::string& text, const Color& fgColor);
    void drawText(Rect rect, const std::string& text);

    inline Pixel* pixelArray() { return pixels; }

    unsigned short width() { return _width; }
    unsigned short height() { return _height; }

private:
    Pixel* pixels;
    unsigned short _width;
    unsigned short _height;
};