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

#include "canvas.hpp"

#include "utf8Utils.hpp"

Canvas::Canvas(short width, short height) {
    this->_width = width;
    this->_height = height;
    pixels = new Pixel[width * height];
}

Canvas::~Canvas() {
    delete[] pixels;
}

std::tuple<short, short> Canvas::getSize() const {
    return std::tuple<short, short>(_width, _height);
}

void Canvas::setPixel(short x, short y, const Pixel& pixel) {
    if (x < 0 || x >= _width || y < 0 || y >= _height)
        throw std::out_of_range("像素坐标超出范围");

    pixels[y * _width + x] = pixel;
}

const Pixel& Canvas::getPixel(short x, short y) const {
    if (x < 0 || x >= _width || y < 0 || y >= _height)
        throw std::out_of_range("像素坐标超出范围");

    return pixels[y * _width + x];
}

void Canvas::fill(Rect rect, const Pixel& pixel) {
    rect.width = std::min<short>(_width - rect.x, rect.width);
    rect.height = std::min<short>(_height - rect.y, rect.height);

    for (int y = rect.y; y < rect.y + rect.height; ++y)
        for (int x = rect.x; x < rect.x + rect.width; ++x)
            pixels[y * _width + x] = pixel;
}

void Canvas::fill(Rect rect, const Color& bgColor) {
    rect.width = std::min<short>(_width - rect.x, rect.width);
    rect.height = std::min<short>(_height - rect.y, rect.height);

    for (int y = rect.y; y < rect.y + rect.height; ++y)
        for (int x = rect.x; x < rect.x + rect.width; ++x)
            pixels[y * _width + x] = Pixel{Color(), bgColor};
}

void Canvas::drawText(Rect rect, const std::string& text, const Color& fgColor, const Color& bgColor) {
    rect.width = std::min<short>(_width - rect.x, rect.width);
    rect.height = std::min<short>(_height - rect.y, rect.height);

    int curBeginByte = 0;
    int curX = rect.x;
    int curY = rect.y;

    for (int i = 0; i < rect.height; ++i) {
        std::string_view row = limitStrPrintWidth(text, rect.width, curBeginByte);
        std::vector<Utf8Char> u8Chars = getUtf8Chars(row);
        for (auto& u8c : u8Chars) {
            Pixel& pix = pixels[curY * _width + curX];
            pix.foregroundColor = fgColor;
            pix.backgroundColor = bgColor;
            pix.character = u8c;

            if (getPrintWidth(u8c) == 1)
                curX++;
            else {
                pixels[curY * _width + curX + 1].character = {0, 0, 0, 0};
                curX += 2;
            }
        }

        curBeginByte += static_cast<int>(row.size());
        curX = rect.x;
        curY++;
    }
}

void Canvas::drawText(Rect rect, const std::string& text, const Color& fgColor) {
    rect.width = std::min<short>(_width - rect.x, rect.width);
    rect.height = std::min<short>(_height - rect.y, rect.height);

    int curBeginByte = 0;
    int curX = rect.x;
    int curY = rect.y;

    for (int i = 0; i < rect.height; ++i) {
        std::string_view row = limitStrPrintWidth(text, rect.width, curBeginByte);
        std::vector<Utf8Char> u8Chars = getUtf8Chars(row);
        for (auto& u8c : u8Chars) {
            Pixel& pix = pixels[curY * _width + curX];
            pix.foregroundColor = fgColor;
            pix.character = u8c;

            if (getPrintWidth(u8c) == 1)
                curX++;
            else {
                pixels[curY * _width + curX + 1].character = {0, 0, 0, 0};
                curX += 2;
            }
        }

        curBeginByte += static_cast<int>(row.size());
        curX = rect.x;
        curY++;
    }
}

void Canvas::drawText(Rect rect, const std::string& text) {
    rect.width = std::min<short>(_width - rect.x, rect.width);
    rect.height = std::min<short>(_height - rect.y, rect.height);

    int curBeginByte = 0;
    int curX = rect.x;
    int curY = rect.y;

    for (int i = 0; i < rect.height; ++i) {
        std::string_view row = limitStrPrintWidth(text, rect.width, curBeginByte);
        std::vector<Utf8Char> u8Chars = getUtf8Chars(row);
        for (auto& u8c : u8Chars) {
            Pixel& pix = pixels[curY * _width + curX];
            pix.character = u8c;

            if (getPrintWidth(u8c) == 1)
                curX++;
            else {
                pixels[curY * _width + curX + 1].character = {0, 0, 0, 0};
                curX += 2;
            }
        }

        curBeginByte += static_cast<int>(row.size());
        curX = rect.x;
        curY++;
    }
}