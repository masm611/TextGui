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

#include "canvasWidget.hpp"
#include "painter.hpp"

CanvasWidget::CanvasWidget(Widget* parent) : Widget(parent) {
}

CanvasWidget::~CanvasWidget() {
}

void CanvasWidget::setPixel(short x, short y, const Pixel& pixel) {
    if (x < 0 || x >= _canvasWidth || y < 0 || y >= _canvasHeight)
        return;

    size_t index = y * _canvasWidth + x;
    _pixels[index] = pixel;
    _pixelSet[index] = true;
}

void CanvasWidget::clearPixel(short x, short y) {
    if (x < 0 || x >= _canvasWidth || y < 0 || y >= _canvasHeight)
        return;

    _pixelSet[y * _canvasWidth + x] = false;
}

void CanvasWidget::setSize(short width, short height) {
    if (width < 0)
        width = 0;
    if (height < 0)
        height = 0;

    _canvasWidth = width;
    _canvasHeight = height;

    size_t totalPixels = width * height;
    _pixels = std::vector<Pixel>(totalPixels);
    _pixelSet = std::vector<bool>(totalPixels, false);
}

void CanvasWidget::clear() {
    for (size_t i = 0; i < _pixelSet.size(); ++i)
        _pixelSet[i] = false;
}

void CanvasWidget::draw() {
    Widget::draw();

    if (_canvasWidth == 0 || _canvasHeight == 0)
        return;

    Painter painter(this);

    for (short y = 0; y < _canvasHeight; ++y)
        for (short x = 0; x < _canvasWidth; ++x)
            if (_pixelSet[y * _canvasWidth + x])
                painter.setPixel(x, y, _pixels[y * _canvasWidth + x]);
}
